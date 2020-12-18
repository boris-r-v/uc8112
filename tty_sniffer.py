#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, sys
import serial 
import signal

ser = serial.Serial( "/dev/ttyUSB0", 19200, timeout=1 )
print ( 'Ctrl+C to exit!' )
print ( ser )

def signal_handler(signal, frame):
    ser.close()
    sys.exit(0)
    signal.signal(signal.SIGINT, signal_handler)


while True:
    print ( "--------------------Iteration Begin-----------------------------------------------------")

    raw = ser.read(255)
    print (raw)
    print ("\n")
#    print (":".join("{:02x}".format(ord(c)) for c in str(raw)) )

#    print ("--------------------Data Expanded by :00------------------------------------------------")

#    ret = ""
#    for rr in raw:
#        if ( ord(rr) == 0 ):
#            if ( ret != None ):
#                print (":".join("{:02x}".format(ord(c)) for c in ret) )
#                ret = rr
#            else:
#                ret += rr
#            raw = None
#
#    print ("--------------------Iteration End-------------------------------------------------------")






