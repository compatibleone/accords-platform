# vim: set et sw=4 ts=4 ai:

import unittest
from testbin import TestBin

class TestBinOsocci(TestBin, unittest.TestCase):

    def setUp(self):
        self.bin = 'testosocci'

    def tearDown(self):
        pass
