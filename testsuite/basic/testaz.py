# vim: set et sw=4 ts=4 ai:

import unittest
from testbin import TestBin

class TestBinAz(TestBin, unittest.TestCase):

    def setUp(self):
        self.bin = 'testaz'

    def tearDown(self):
        pass
