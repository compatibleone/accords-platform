# vim: set et sw=4 ts=4 ai:

import unittest
import utils
from testbin import TestBin

class TestBinConets(TestBin, unittest.TestCase):

    def setUp(self):
        self.bin = 'conets'

    def tearDown(self):
        pass
