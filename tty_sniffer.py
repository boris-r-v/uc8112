#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os, sys
import serial 
import signal

tty="/dev/ttyUSB0"

ser = serial.Serial( tty, 19200, timeout=0.25, stopbits=serial.STOPBITS_TWO )
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
    print ("RecData:(", tty, ")", request )

