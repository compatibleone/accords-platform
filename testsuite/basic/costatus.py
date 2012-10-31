# vim: set et sw=4 ts=4 ai:

import unittest
import utils
from testbin import TestBin

class TestBinCostatus(TestBin, unittest.TestCase):

    def setUp(self):
        self.bin = 'co-status'

    def tearDown(self):
        pass
