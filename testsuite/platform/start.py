# vim: set et sw=4 ts=4 ai:

import subprocess
try:
    # Python2.6 has no assertIn for unittest
    # unittest2 is a backport from 2.7 to 2.6
    import unittest2
    clss = unittest2.TestCase
except ImportError:
    import unittest
    clss = unittest.TestCase
import psutil

from binaries import cobinaries, coprocesses, coreduce

class TestPlatformStart(clss):

    def setUp(self):
        pass

    def test_platform_start(self):
        subprocess.call(['/usr/bin/co-start'])
        binaries = cobinaries.get_binaries()
        processes = coprocesses.get_list()
        reduced = coreduce.reduce(binaries, processes)
        self.assertItemsEqual(binaries, reduced)

    def tearDown(self):
        subprocess.call(['/usr/bin/co-stop'])
