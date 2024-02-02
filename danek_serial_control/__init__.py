import struct
import serial
import datetime
from time import sleep


class DanekSerialController:
    def __init__(self, com_port='COM9', baudrate=9600):
        self._port = serial.Serial(com_port, baudrate)

    def _write_command(self, command_type, param_number, value):
        self._port.write(struct.pack('bbh', command_type, param_number, value))
        sleep(0.05)


    def set_current_time(self):
        self._write_command(1, 0, datetime.datetime.now().hour)
        self._write_command(1, 1, datetime.datetime.now().minute)
        self._write_command(1, 2, datetime.datetime.now().second)

    def enable_rgb_light(self, r, g, b):
        self._write_command(1, 3, 1)

    def set_rgb_light(self, r, g, b):
        self._write_command(1, 4, r)
        self._write_command(1, 5, g)
        self._write_command(1, 6, b)
        self._write_command(1, 7, 0)

    def disable_rgb_light(self):
        self._write_command(1, 3, 0)

