#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, sys
import serial 
import signal

ser = serial.Serial( "/dev/ttyUSB0", 19200, timeout=0.25 )
print ( 'Ctrl+C to exit!' )
print ( ser )

def signal_handler(signal, frame):
    ser.close()
    sys.exit(0)
    signal.signal(signal.SIGINT, signal_handler)


while True:
    print ( "--------------------Iteration-----------------------------------------------------")

    raw = ser.read(255)
    request = " ".join("".join(hex(c)) for c in raw)
    print (f"Request: {request}")

