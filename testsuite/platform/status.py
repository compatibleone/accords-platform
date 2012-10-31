# vim: set et sw=4 ts=4 ai:

import re
import subprocess
try:
    # Python2.6 has no assertIn for unittest
    # unittest2 is a backport from 2.7 to 2.6
    import unittest2
    clss = unittest2.TestCase
except ImportError:
    import unittest
    clss = unittest.TestCase
from binaries import cobinaries, coprocesses, coreduce

class TestPlatformStatus(clss):

    def setUp(self):
        subprocess.call(['/usr/bin/co-start'])

    def test_platform_status(self):
        netstatRe = re.compile(r'.*LISTEN.*\/(\w+)')
        process = subprocess.Popen(['/usr/bin/co-status'], stdout=subprocess.PIPE)
        out, err = process.communicate()
        status = filter(
            None,
            map(lambda x: x and x.group(1),
                map(lambda x: netstatRe.match(x), out.split('\n'))
               )
            )
        processes = coprocesses.get_list()
        reduced = coreduce.reduce(status, processes)
        self.assertItemsEqual(status, reduced)

    def tearDown(self):
        subprocess.call(['/usr/bin/co-stop'])
