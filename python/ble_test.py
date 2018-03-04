import argparse
import logging
import sys
import threading

from bluepy import btle

log = None
NAME_DESCRIPTOR = "Complete Local Name"
NAME = "Empty Example"

def handle_device_connection(addr):
    log.info("Handling device")
    log.info("Attempting to connect to: {}".format(addr))
    dev = btle.Peripheral(addr)

    log.info("Here are a list of services")
    for svc in dev.services:
        log.info(svc)

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

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("--verbosity", "-v", type=str, choices=["DEBUG", "INFO"],
         default="INFO", help="Level of verbosity")
    args = parser.parse_args()

    FORMAT = '[%(asctime)s %(filename)15s line: %(lineno)5s] %(message)s'
    logging.basicConfig(stream=sys.stdout, format=FORMAT, level=getattr(logging, args.verbosity))
    log = logging.getLogger("ble_test")

    scanner = btle.Scanner(0).withDelegate(ScanPrint())
    devices = scanner.scan(25)
