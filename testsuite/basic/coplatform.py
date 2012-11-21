# vim: set et sw=4 ts=4 ai:

import unittest
import utils
from testbin import TestBin

class TestBinCoplatform(TestBin, unittest.TestCase):

    def setUp(self):
        self.bin = 'co-platform'
        self.do_not_test_running = True

    def tearDown(self):
        pass
