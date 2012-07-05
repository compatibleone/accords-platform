import os
import socket 
import subprocess
import signal
import sys
import logging
import time
import miscco
import platform

# logging system
curros=platform.platform()
currdir=os.getcwd()
hostname=socket.gethostname()
lockfile=currdir + '/lock-' + hostname
logfile=currdir + '/log-cosacs-wrapper-' + hostname
logger = miscco.init_logger(logfile)

logger.info('----------')
logger.info('The current directory is ' + currdir)
logger.info('The current hostname is ' + hostname)
logger.info('The lockfile is in ' + lockfile)
logger.info('The current platform is ' + curros)

def signal_handler(signal, frame):
    logger.warn('Signal: i am not dying at all.')

signal.signal(signal.SIGINT, signal_handler)
signal.signal(signal.SIGTERM, signal_handler)

miscco.touch_file(lockfile)

while 1:
    if miscco.is_cosacs_running(): # COSACS IS running
        if os.path.exists(lockfile):
            # All right
            time.sleep(1) 
        else:
            miscco.touch_file(lockfile)
            logger.info('Created lockfile.')
    else:                   # COSACS is NOT running
        if os.path.exists(lockfile):
            os.remove(lockfile)
            logger.info('Removed lockfile.')
        else:
            # All right
            time.sleep(1) 

    
logger.info('Created lockfile: ' + lockfile)
logger.info('Waiting for COSACS to die...')
miscco.block_while_cosacs_runs()
logger.info('COSACS died... Removing lockfile: ' + lockfile)
os.remove(lockfile)
logger.info('Done.')

