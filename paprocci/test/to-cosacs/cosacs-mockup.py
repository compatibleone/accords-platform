import os
import socket 
import subprocess
import signal
import sys
import logging
import time

#print 'The current directory is' + os.getcwd()
addr=('127.0.0.1', 8286)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
    #print 'Simulating COSACS...'
    sock.bind(addr)
    time.sleep(200)
    sock.close()
    #print 'done.'
except:
    #print 'COSACS is already running...'
    a=1
