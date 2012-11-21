# vim: set et sw=4 ts=4 ai:

import unittest
import utils
from testbin import TestBin

class TestBinFileserver(TestBin, unittest.TestCase):

    def setUp(self):
        self.bin = 'fileserver'

    def tearDown(self):
        pass
