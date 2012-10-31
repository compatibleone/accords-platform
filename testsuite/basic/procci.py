# vim: set et sw=4 ts=4 ai:

import unittest
import utils
from testbin import TestBin

class TestBinProcci(TestBin, unittest.TestCase):

    def setUp(self):
        self.bin = 'procci'

    def tearDown(self):
        pass
