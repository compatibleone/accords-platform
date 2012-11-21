# vim: set et sw=4 ts=4 ai:

import unittest
from testbin import TestBin

class TestBinResolver(TestBin, unittest.TestCase):

    def setUp(self):
        self.bin = 'testresolver'

    def tearDown(self):
        pass
