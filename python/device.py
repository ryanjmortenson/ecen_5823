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
CHAR_SOIL_MOISTURE_UUID = "83c77eb6-35af-4dd9-8851-87d0a92ea404"

# Locking mechanism to sequentialize access to BTLE devices
ble_lock = threading.Lock()

# Keep a list of currently active devices
valid_list = dict()

def find_devices(intf, meas_queue, timeout=10):
    log = logging.getLogger("ble_test")
    scanner = btle.Scanner(intf)
    try:
        ble_lock.acquire()
        devices = scanner.scan(timeout)
        ble_lock.release()
        for dev in devices:
            if dev.addr not in valid_list.keys():
                SensorDevice(dev.addr, meas_queue).initial_connection()
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

    def __init__(self, addr, meas_queue):
        '''
        Constructor
        '''
        self.log = logging.getLogger("ble_test")
        self.log.info("Initializing SensorDevice for addr {}".format(addr))
        self.addr = addr
        self.temp_handle = None
        self.lux_handle = None
        self.soil_handle = None
        self.char_uuid_list = [CHAR_TEMP_UUID, CHAR_SOIL_MOISTURE_UUID, CHAR_LUX_UUID]
        self.meas_queue = meas_queue
        self.shutdown = threading.Event()

    def initial_connection(self):
        '''
        Connect to device and check if it has the required characteristics.  If
        device has characteristics start a thread that polls the device every
        60 seconds
        '''

        dev = None
        start_thread = False
        ble_lock.acquire()

        # Unfortunately with the RPi there needs to be a retry mechanism
        for x in range(0, 5):
            try:
                dev = btle.Peripheral(self.addr)
                self.set_handles(dev)
                dev.disconnect()
                valid_list[self.addr] = self
                start_thread = True
                break
            except:
                if dev is not None:
                    dev.disconnect()
                self.log.error("Could not connect to {} on try {}".format(self.addr, x + 1))
                if x + 1 == 5:
                    self.log.error(traceback.format_exc())
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
        while count < time and not self.shutdown.is_set():
            sleep(1)
            count += 1

    def read_info_periodically(self):
        '''
        Poll device every 60 seconds for sensor measurements
        '''

        failures = 0
        while not self.shutdown.is_set():
            dev = None
            try:
                self.polling_sleep(10)
                ble_lock.acquire()
                self.log.info("Reading information from {}".format(self.addr))
                dev = btle.Peripheral(self.addr)
                temp_char = dev.readCharacteristic(self.temp_handle)
                soil_char = dev.readCharacteristic(self.soil_handle)
                lux_char = dev.readCharacteristic(self.lux_handle)
                dev.disconnect()
                ble_lock.release()
                meas = Measurement(self.addr, temp_char, soil_char, lux_char)
                self.meas_queue.put(meas)
                failures = 0
                self.log.info("Done {}".format(self.addr))
            except:
                failures += 1
                if dev is not None:
                    dev.disconnect()
                ble_lock.release()
                self.log.error("Could not get sensor info from {}".format(self.addr))
                self.log.error("Current failures count is {}".format(failures))
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

        self.temp_handle = temp_char.getHandle()
        self.lux_handle = lux_char.getHandle()
        self.soil_handle = soil_char.getHandle()

        self.log.debug("Temp handle: {}".format(self.temp_handle))
        self.log.debug("Lux handle: {}".format(self.lux_handle))
        self.log.debug("Soil Moisture handle: {}".format(self.soil_handle))
