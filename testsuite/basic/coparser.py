# vim: set et sw=4 ts=4 ai:

import unittest
import utils
from testbin import TestBin

class TestBinCoparser(TestBin, unittest.TestCase):

    def setUp(self):
        self.bin = 'co-parser'

    def tearDown(self):
        pass
