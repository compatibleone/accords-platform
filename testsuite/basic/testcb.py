# vim: set et sw=4 ts=4 ai:

import unittest
from testbin import TestBin

class TestBinCb(TestBin, unittest.TestCase):

    def setUp(self):
        self.bin = 'testcb'

    def tearDown(self):
        pass
