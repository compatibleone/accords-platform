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

from binaries import cobinaries, coprocesses, coreduce

class TestPlatformStop(clss):

    def setUp(self):
        subprocess.call(['/usr/bin/co-start'])

    def test_platform_stop(self):
        subprocess.call(['/usr/bin/co-stop'])
        processes = coprocesses.get_list()
        binaries = cobinaries.get_binaries()
        reduced = coreduce.reduce(binaries, processes)
        self.assertItemsEqual([], reduced)

    def tearDown(self):
        pass
