import argparse
import logging
import sys

from bluepy import btle

log = None

class ScanPrint(btle.DefaultDelegate):

    def __init__(self):
        btle.DefaultDelegate.__init__(self)

    def handleDiscovery(self, dev, isNewDev, isNewData):
        log.info(dev.getScanData())

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("bssid", type=str)
    parser.add_argument("--verbosity", "-v", type=str, choices=["DEBUG", "INFO"],
         default="INFO", help="Level of verbosity")
    args = parser.parse_args()

    FORMAT = '[%(asctime)s %(filename)15s line: %(lineno)5s] %(message)s'
    logging.basicConfig(stream=sys.stdout, format=FORMAT, level=getattr(logging, args.verbosity))
    log = logging.getLogger("ble_test")

    scanner = btle.Scanner(0).withDelegate(ScanPrint())
    devices = scanner.scan(5)

    log.info(len(devices))

    '''
    log.info("Attempting to connect to: {}".format(args.bssid))
    dev = btle.Peripheral(args.bssid)

    log.info("Here are a list of services")
    for svc in dev.services:
        log.info(svc)
    '''
