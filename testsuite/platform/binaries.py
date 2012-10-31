# vim: set et sw=4 ts=4 ai:

import re
import psutil

class COBinaries():
    
    def __init__(self):
        self.binaries = []
        self.runregex = re.compile(r'_co_run_serv (.*) ".*"')

        f = file('/usr/bin/accords')
        for line in file.readlines(f):
            match = self.runregex.search(line)
            if match:
                self.binaries += [ match.group(1) ]
        f.close()

    def get_binaries(self):
        return self.binaries

class COProcesses():

    def __init__(self):
        pass

    def get_list(self):
        return map(lambda x: x.name, psutil.get_process_list())

class COReduce():

    def __init__(self):
        pass

    def reduce(self, l1, l2):
        return list(set.intersection(set(l1), set(l2)))

cobinaries = COBinaries()
coprocesses = COProcesses()
coreduce = COReduce()
