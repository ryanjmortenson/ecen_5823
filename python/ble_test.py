import sys
sys.path.insert(0, "/home/pi/Workspace/Cayenne-MQTT-Python")
import argparse
import logging
import sys
import traceback
import Queue

from bluepy import btle
from time import sleep
from device import find_devices, cleanup_devices
from client import CayenneClient

log = None
client = None


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--verbosity", "-v", type=str, choices=["DEBUG", "INFO"],
                        default="INFO", help="Level of verbosity")
    parser.add_argument("--interface", "-i", type=int, default=0,
                        help="Interface to use fot BLE")
    parser.add_argument("--scan_timeout", "-s", type=int, default=10,
                        help="How man seconds to BLE scan")
    parser.add_argument("--scan_period", "-p", type=int, default=30,
                        help="Periodicity of BLE scan in seconds (doesn't include scan timeout")
    parser.add_argument("--poll_period", "-l", type=int, default=300,
                        help="Periodicity of BLE sensor reading")
    args = parser.parse_args()

    FORMAT = '[%(asctime)s %(filename)15s line: %(lineno)5s] %(message)s'
    logging.basicConfig(stream=sys.stdout, format=FORMAT, level=getattr(logging, args.verbosity))
    log = logging.getLogger("ble_test")

    meas_queue = Queue.Queue()
    new_client = CayenneClient(meas_queue)
    new_client.begin()
    try:
        while True:
            log.info("Starting Scan with timeout {} secs ...".format(args.scan_timeout))
            find_devices(args.interface, meas_queue, args.scan_timeout, args.poll_period)
            log.info("Scan Complete, sleeping for {} seconds".format(args.scan_period))
            sleep(args.scan_period)
    except:
        new_client.end()
        cleanup_devices()
