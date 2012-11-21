# vim: set et sw=4 ts=4 ai:

import unittest
from testbin import TestBin

class TestBinOs(TestBin, unittest.TestCase):

    def setUp(self):
        self.bin = 'testos'

    def tearDown(self):
        pass
