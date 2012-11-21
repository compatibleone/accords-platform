# vim: set et sw=4 ts=4 ai:

import unittest
import utils
from testbin import TestBin

class TestBinAccords(TestBin, unittest.TestCase):

    def setUp(self):
        self.bin = 'accords'
        self.return_value = 1

    def tearDown(self):
        pass
