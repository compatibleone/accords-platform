# vim: set et sw=4 ts=4 ai:

import unittest
import utils
from testbin import TestBin

class TestBinOn(TestBin, unittest.TestCase):

    def setUp(self):
        self.bin = 'teston'

    def tearDown(self):
        pass
