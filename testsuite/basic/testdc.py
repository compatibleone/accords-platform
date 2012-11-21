# vim: set et sw=4 ts=4 ai:

import unittest
from testbin import TestBin

class TestBinDc(TestBin, unittest.TestCase):

    def setUp(self):
        self.bin = 'testdc'

    def tearDown(self):
        pass
