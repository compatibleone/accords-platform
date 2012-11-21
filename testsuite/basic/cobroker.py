# vim: set et sw=4 ts=4 ai:

import unittest
import utils
from testbin import TestBin

class TestBinCobroker(TestBin, unittest.TestCase):

    def setUp(self):
        self.bin = 'co-broker'

    def tearDown(self):
        pass
