import argparse
import logging
import threading
import traceback
import struct
from meas import Measurement

from bluepy import btle
from time import sleep

# Characteristics to read
CHAR_TEMP_UUID = "00002a6e-0000-1000-8000-00805f9b34fb"
CHAR_LUX_UUID = "00002a77-0000-1000-8000-00805f9b34fb"
CHAR_HUM_UUID = "00002a6f-0000-1000-8000-00805f9b34fb"
CHAR_SOIL_MOISTURE_UUID = "83c77eb6-35af-4dd9-8851-87d0a92ea404"
CHAR_CONN_COUNT_UUID = "79556713-85bd-4e5f-944d-7363449d1baa"
CHAR_MEAS_COUNT_UUID = "016d95b5-006f-4e86-9588-bd551a6b1c6d"

# Device Name
DEVICE_NAME = "BLE IOP"

# Locking mechanism to sequentialize access to BTLE devices
ble_lock = threading.Lock()

# Keep a list of currently active devices
valid_list = dict()

def find_devices(intf, meas_queue, timeout=10, poll_period=300):
    log = logging.getLogger("ble_test")
    scanner = btle.Scanner(intf)
    try:
        ble_lock.acquire()
        devices = scanner.scan(timeout)
        ble_lock.release()
        for dev in devices:
            for item in dev.getScanData():
                is_device_name = (str(item[2]).find(DEVICE_NAME) != -1)
                if is_device_name and dev.addr not in valid_list.keys():
                    SensorDevice(dev.addr,
                                 meas_queue,
                                 poll_period).initial_connection()
    except KeyboardInterrupt as e:
        ble_lock.release()
        raise KeyboardInterrupt
    except Exception as e:
        ble_lock.release()
        log.error(traceback.format_exc())
        log.error("Encountered an issue scanning");

def cleanup_devices():
    log = logging.getLogger("ble_test")
    for device in valid_list.keys():
        log.debug("Shutting down {}".format(device))
        valid_list[device].shutdown.set()

class SensorDevice(object):
    '''
    Sensor device (Server) information
    '''

    def __init__(self, addr, meas_queue, sleep_time=300):
        '''
        Constructor
        '''
        self.log = logging.getLogger("ble_test")
        self.log.info("Initializing SensorDevice for addr {}".format(addr))
        self.addr = addr
        self.meas_queue = meas_queue
        self.shutdown = threading.Event()
        self.sleep_time = sleep_time

        # Required Handles for device
        self.temp_handle = None
        self.hum_handle = None
        self.lux_handle = None
        self.soil_handle = None
        self.meas_handle = None
        self.conn_handle = None

    def initial_connection(self):
        '''
        Connect to device and check if correct UUIDs exist
        '''

        dev = None
        start_thread = False
        ble_lock.acquire()
        try:
            dev = btle.Peripheral(self.addr)
            self.set_handles(dev)
            dev.disconnect()
            valid_list[self.addr] = self
            start_thread = True
        except:
            if dev is not None:
                dev.disconnect()
        ble_lock.release()

        if start_thread:
            try:
                t = threading.Thread(target=self.read_info_periodically)
                t.start()
            except:
                self.log.error("Could not start new thread {}".format(self.addr))
                self.log.error(traceback.format_exc())

    def polling_sleep(self, time):
        count = 0
        self.log.debug("Sleeping for {}".format(time))
        while count < time and not self.shutdown.is_set():
            sleep(1)
            count += 1

        if self.shutdown.is_set():
            return False
        else:
            return True

    def read_info_periodically(self):
        '''
        Poll device every 60 seconds for sensor measurements
        '''

        failures = 0
        sleep_time = self.sleep_time
        while not self.shutdown.is_set():
            dev = None
            try:
                if not self.polling_sleep(sleep_time):
                    break
                ble_lock.acquire()
                self.log.info("Reading information from {}".format(self.addr))
                dev = btle.Peripheral(self.addr)
                # Hackery so the Authentication process is completed before trying to read
                # the first characteristic
                sleep(2)
                soil_char = dev.readCharacteristic(self.soil_handle)
                temp_char = dev.readCharacteristic(self.temp_handle)
                hum_char = dev.readCharacteristic(self.hum_handle)
                lux_char = dev.readCharacteristic(self.lux_handle)
                conn_char = dev.readCharacteristic(self.conn_handle)
                meas_char = dev.readCharacteristic(self.meas_handle)
                dev.disconnect()
                self.log.info("Done {}".format(self.addr))
                ble_lock.release()
                meas = Measurement(self.addr,
                                   temp_char,
                                   hum_char,
                                   lux_char,
                                   soil_char,
                                   conn_char,
                                   meas_char)
                self.meas_queue.put(meas)
                failures = 0
                sleep_time = self.sleep_time
            except:
                failures += 1
                if dev is not None:
                    dev.disconnect()
                ble_lock.release()
                self.log.error("Could not get sensor info from {}".format(self.addr))
                self.log.error("Current failures count is {}".format(failures))
                sleep_time = 10
            finally:
                if failures == 5:
                    msg = "Device {} could not connect 5 times in a row"
                    self.log.error(msg.format(self.addr))
                    valid_list.pop(self.addr)
                    break

    def set_handles(self, dev):
        '''
        Get the handles to the characteris on the device
        '''

        # Get all of the handles
        temp_char = dev.getCharacteristics(uuid=CHAR_TEMP_UUID)[0]
        soil_char = dev.getCharacteristics(uuid=CHAR_SOIL_MOISTURE_UUID)[0]
        lux_char = dev.getCharacteristics(uuid=CHAR_LUX_UUID)[0]
        hum_char = dev.getCharacteristics(uuid=CHAR_HUM_UUID)[0]
        meas_char = dev.getCharacteristics(uuid=CHAR_MEAS_COUNT_UUID)[0]
        count_char = dev.getCharacteristics(uuid=CHAR_CONN_COUNT_UUID)[0]
        self.temp_handle = temp_char.getHandle()
        self.lux_handle = lux_char.getHandle()
        self.hum_handle = hum_char.getHandle()
        self.soil_handle = soil_char.getHandle()
        self.meas_handle = meas_char.getHandle()
        self.conn_handle = count_char.getHandle()

        # Log handles
        self.log.debug("Temp handle: {}".format(self.temp_handle))
        self.log.debug("Humidity handle: {}".format(self.hum_handle))
        self.log.debug("Lux handle: {}".format(self.lux_handle))
        self.log.debug("Soil Moisture handle: {}".format(self.soil_handle))
        self.log.debug("Measurement count handle: {}".format(self.meas_handle))
        self.log.debug("Connection count handle: {}".format(self.conn_handle))
