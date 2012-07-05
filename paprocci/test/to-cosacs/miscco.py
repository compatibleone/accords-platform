import os
import socket 
import subprocess
import signal
import sys
import logging
import time

# - execute a process
#print 'executing process 1...'
#executable='sleep'
#param='3'
#subprocess.call([executable, param])
#print 'executing process 2...'
#os.system('sleep 1')
# - capture SIGINT and other signals 
#print 'done.'

def init_logger(filename):
    logger = logging.getLogger(filename)
    hdlr = logging.FileHandler(filename)
    formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
    hdlr.setFormatter(formatter)
    logger.addHandler(hdlr)
    logger.setLevel(logging.INFO)
    return logger

def touch_file(file_name):
    f = open(file_name,'w')
    f.close

# - detect COSACS process running
def is_cosacs_running():
    addr=('127.0.0.1', 8286)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        sock.bind(addr)
        sock.close()
        return 0
    except:
        return 1

def block_while_file_exists(filename):
    while 1: 
        if os.path.exists(filename):
            time.sleep(1)
        else:
            break

def block_while_cosacs_runs():
    while 1: 
        if is_cosacs_running():
            time.sleep(1)
        else:
            break

