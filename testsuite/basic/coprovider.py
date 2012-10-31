# vim: set et sw=4 ts=4 ai:

import unittest
import utils
from testbin import TestBin

class TestBinCoprovider(TestBin, unittest.TestCase):

    def setUp(self):
        self.bin = 'co-provider'

    def tearDown(self):
        pass
