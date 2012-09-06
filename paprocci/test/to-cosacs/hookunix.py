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
logfile=currdir + '/log-hook-' + hostname
logger = miscco.init_logger(logfile)

logger.info('----------')
logger.info('The current directory is ' + currdir)
logger.info('The current hostname is ' + hostname)
logger.info('The lockfile is in ' + lockfile)
logger.info('The current platform is ' + curros)
logger.info('The arguments are ')
command = []
first = 0
for arg in sys.argv:
    if first==0:
        first = 1
    else:
        logger.info('  ' + arg)
        command.append(arg)

def signal_handler(signal, frame):
    logger.info('Signal: something tryied to kill me... I am not dying...')

signal.signal(signal.SIGINT, signal_handler)

logger.info('Running process...')
process = subprocess.Popen(command)

logger.info('Process is running with PID ' + str(process.pid) + ' ... Let it be for a while... ')

time.sleep(20) # In a perfect world, NFS would synchronize all the files in all the nodes of a grid in 0 seconds. 

logger.info('Watching lockfile...')
miscco.block_while_file_exists(lockfile)

logger.info('File deleted. Process will be killed...')
os.kill(process.pid, 9)
logger.info('Done.')

