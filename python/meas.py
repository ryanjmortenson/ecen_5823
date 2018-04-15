import struct

class Measurement(object):
    '''
    Class for holding and converting measurements
    '''

    def __init__(self, addr, temp, lux, soil, conn, meas):
        '''
        Constructor
        '''
        self.temp = temp
        self.lux = lux
        self.soil = soil
        self.addr = addr
        self.conn = conn
        self.meas = meas

    def get_temp(self):
        '''
        Transform bytes to short
        '''
        return float(struct.unpack("<h", self.temp)[0])/float(1000)

    def get_soil_moisture(self):
        '''
        Transform bytes to short
        '''
        return float(struct.unpack("<h", self.soil)[0])/float(4096) * 100

    def get_lux(self):
        '''
        Transform bytes to int
        '''
        return struct.unpack("<i", self.lux)[0]

    def get_connection_count(self):
        '''
        Transform bytes to int
        '''
        return struct.unpack("<i", self.conn)[0]

    def get_measurement_count(self):
        '''
        Transform bytes to int
        '''
        return struct.unpack("<i", self.meas)[0]

    def get_address(self):
        '''
        Return address where measurement is taken 
        '''
        return self.addr
