import argparse
import logging
import sys
import threading
import cayenne.client
import traceback

from bluepy import btle
from time import sleep

NAME_DESCRIPTOR = "Complete Local Name"
NAME = "Empty Example"
MQTT_USERNAME  = "a465aea0-2006-11e8-b59c-db84183bf26b"
MQTT_PASSWORD  = "6ce6adfd408260f4d98511e1658bfc96fcf85313"
MQTT_CLIENT_ID = "224bc110-2007-11e8-9f38-9fae3d42ebf0"

CHAR_TEMP_UUID = "00002a6e-0000-1000-8000-00805f9b34fb"
CHAR_SOIL_MOISTURE_UUID = "83c77eb6-35af-4dd9-8851-87d0a92ea404"

log = None
client = None

def handle_device_connection(addr):
    log.info("Attempting to connect to: {}".format(addr))
    try:
        dev = btle.Peripheral(addr)
    except Exception as e:
        log.error("Could not connect to device: {}".format(addr))
        return

    try:
        '''
        log.info("Here are a list of services for {}".format(addr))
        for svc in dev.services:
            log.info(svc)
            log.info("UUID: {}".format(svc.uuid))
            for char in svc.getCharacteristics():
                log.info("\t bssid {}: {}".format(addr, str(char)))
                if char.supportsRead():
                    val = char.read()
                    if val.isalpha():
                        log.info("\t\tbssid: {} value: {}".format(val))
                    else:
                        val = int(val[-1::-1].encode("hex"), 16)
                        log.info("\t\tbssid: {} value (int): {}".format(addr, val))
        '''
        temp_char = dev.getCharacteristics(uuid=CHAR_TEMP_UUID)[0]
        soil_char = dev.getCharacteristics(uuid=CHAR_SOIL_MOISTURE_UUID)[0]
        temp = int(temp_char.read()[-1::-1].encode("hex"), 16)
        soil = int(soil_char.read()[-1::-1].encode("hex"), 16)
        client.celsiusWrite(addr + " TEMP", float(temp)/float(1000))
        client.virtualWrite(addr + " MOISTURE", (float(soil)/float(4096) * 100), "Soil Moisture", "%")
        log.info(temp)
        log.info(soil)
        log.info("Done listing items, disconnecting")
        dev.disconnect()
    except Exception as e:
        log.error("Lost connection")
        log.error(traceback.format_exc())

class ScanPrint(btle.DefaultDelegate):

    def __init__(self):
        btle.DefaultDelegate.__init__(self)

    def handleDiscovery(self, dev, isNewDev, isNewData):
        scan_data = dev.getScanData()
        for (sdid, desc, val) in scan_data:
            if desc == NAME_DESCRIPTOR and val == NAME:
                log.info("Found device {} reading values".format(dev.addr))
                t = threading.Thread(target=handle_device_connection, args=(dev.addr, ))
                t.start()

def on_message(message):
      print("message received: " + str(message))

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("--verbosity", "-v", type=str, choices=["DEBUG", "INFO"],
         default="INFO", help="Level of verbosity")
    args = parser.parse_args()

    FORMAT = '[%(asctime)s %(filename)15s line: %(lineno)5s] %(message)s'
    logging.basicConfig(stream=sys.stdout, format=FORMAT, level=getattr(logging, args.verbosity))
    log = logging.getLogger("ble_test")

    client = cayenne.client.CayenneMQTTClient()
    client.on_message = on_message
    client.begin(MQTT_USERNAME, MQTT_PASSWORD, MQTT_CLIENT_ID)

    while True:
        client.loop()
        log.info("Starting Scan...")
        scanner = btle.Scanner(0).withDelegate(ScanPrint())
        devices = scanner.scan(30)
        log.info("Scan Complete, sleeping for 30 seconds")
        sleep(30)
