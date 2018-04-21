import logging
import threading
import cayenne.client
import traceback

from time import sleep
from Queue import Empty

MQTT_USERNAME  = "a465aea0-2006-11e8-b59c-db84183bf26b"
MQTT_PASSWORD  = "6ce6adfd408260f4d98511e1658bfc96fcf85313"
MQTT_CLIENT_ID = "224bc110-2007-11e8-9f38-9fae3d42ebf0"

def on_message(message):
      print("message received: " + str(message))

class CayenneClient(object):
    '''
    Pushes data the MyDevice Cayenne Service
    '''

    def __init__(self, queue):
        '''
        Constructor
        '''
        self.log = logging.getLogger("ble_test")
        self.log.info("Initializing Cayenne Client")
        self.queue = queue
        self.client = cayenne.client.CayenneMQTTClient()
        self.client.on_message = on_message
        self.client.begin(MQTT_USERNAME, 
                          MQTT_PASSWORD, 
                          MQTT_CLIENT_ID, 
                          logname="ble_test", 
                          loglevel=logging.CRITICAL)
        self.shutdown = threading.Event()

    def begin(self):
        '''
        Start thread to pull data off the queue
        '''
        t = threading.Thread(target=self.main_function)
        t.start()

    def main_function(self):
        '''
        Pull data off from queue and push to MyDevice Cayenne Service
        '''
        self.client.loop()
        while not self.shutdown.is_set():
            self.client.loop()
            try:
                measurement = self.queue.get(timeout=1)
                self.post_measurement(measurement)
            except Empty as e:
                continue
            except Exception as e:
                self.log.error(traceback.format_exc())

        self.log.info("Shutting down")

    def end(self):
        '''
        Kill the queue reading thread
        '''
        self.log.info("Setting shutdown")
        self.shutdown.set()

    def post_measurement(self, measurement):
        '''
        Get the integer representations of the data write Cayenne service
        '''
        temp = measurement.get_temp()
        hum = measurement.get_hum()
        soil = measurement.get_soil_moisture()
        lux = measurement.get_lux()
        addr = measurement.get_address()
        conn = measurement.get_connection_count()
        meas = measurement.get_measurement_count()

        self.log.debug("Temp: {}".format(temp))
        self.log.debug("Humidity: {}".format(hum))
        self.log.debug("Soil Moisture: {}".format(soil))
        self.log.debug("Lux : {}".format(lux))
        self.log.debug("Connection Count: {}".format(conn))
        self.log.debug("Measurement Count: {}".format(meas))
        self.log.debug("Address : {}".format(addr))

        self.client.celsiusWrite(hash(addr + " TEMP"), temp)
        self.client.virtualWrite(hash(addr + " HUMIDITY"), hum, "RH", "%")
        self.client.virtualWrite(hash(addr + " MOISTURE"), soil, "Soil Moisture", "%")
        self.client.virtualWrite(hash(addr + " LUX"), lux, "Illuminance", "Lux")
        self.client.virtualWrite(hash(addr + " CONN"), conn, "Connection Count", "Count")
        self.client.virtualWrite(hash(addr + " MEAS"), meas, "Measurement Count", "Count")
