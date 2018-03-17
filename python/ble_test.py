import sys

sys.path.insert(0, "/home/pi/Workspace/Cayenne-MQTT-Python-Fork")

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
CHAR_LUX_UUID = "00002a77-0000-1000-8000-00805f9b34fb"
CHAR_SOIL_MOISTURE_UUID = "83c77eb6-35af-4dd9-8851-87d0a92ea404"

log = None
client = None

class ConnectToDiscoveredDevice(btle.DefaultDelegate):
    def __init__(self):
        btle.DefaultDelegate.__init__(self)

    def handleDiscovery(self, dev, isNewDev, isNewData):
        scan_data = dev.getScanData()
        for (sdid, desc, val) in scan_data:
            if desc == NAME_DESCRIPTOR and val == NAME and isNewDev:
                t = threading.Thread(target=handle_device_connection, args=(dev.addr, ))
                t.start()

def handle_device_connection(addr):
    log.info("Attempting to connect to: {}".format(addr))
    count = 0
    connection = False

    while count < 5 and connection == False:
        try:
            dev = btle.Peripheral(addr)
            temp_char = dev.getCharacteristics(uuid=CHAR_TEMP_UUID)[0]
            soil_char = dev.getCharacteristics(uuid=CHAR_SOIL_MOISTURE_UUID)[0]
            lux_char = dev.getCharacteristics(uuid=CHAR_LUX_UUID)[0]
            temp = int(temp_char.read()[-1::-1].encode("hex"), 16)
            lux = int(lux_char.read()[-1::-1].encode("hex"), 16)
            soil = int(soil_char.read()[-1::-1].encode("hex"), 16)
            client.celsiusWrite(hash(addr + " TEMP"), float(temp)/float(1000))
            client.virtualWrite(hash(addr + " MOISTURE"), (float(soil)/float(4096) * 100), "Soil Moisture", "%")
            client.virtualWrite(hash(addr + " LUX"), lux, "Illuminance", "Lux")
            log.debug("TEMP: {}".format(temp))
            log.debug("SOIL MOISTURE: {}".format(soil))
            log.debug("LUX: {}".format(lux))
            dev.disconnect()
            connection = True
        except Exception as e:
            log.error("Reading characteristics failed on attempt: {}".format(count))
            count += 1

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
    client.begin(MQTT_USERNAME, MQTT_PASSWORD, MQTT_CLIENT_ID, logname="ble_test", loglevel=logging.CRITICAL)

    client.loop()
    while True:
        client.loop()
        log.info("Starting Scan...")
        scanner = btle.Scanner(0).withDelegate(ConnectToDiscoveredDevice())
        devices = scanner.scan(30)
        log.info("Scan Complete, sleeping for 30 seconds")
        sleep(30)
