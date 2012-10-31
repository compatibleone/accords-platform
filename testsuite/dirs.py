# vim: set et sw=4 ts=4 ai:

import os
import unittest

class TestDirs(unittest.TestCase):

    def setUp(self):
        self.assertEqual(os.path.abspath(os.curdir), '/var/lib/accords')

    def tearDown(self):
        pass

    def test_rest(self):
        self.assertTrue(os.path.isdir('rest'))

    def test_security(self):
        self.assertTrue(os.path.isdir('security'))

    def test_accords(self):
        self.assertTrue(os.path.isfile('accords.xml'))

    def test_cords_user(self):
        self.assertTrue(os.path.isfile('cords_user.xml'))

    def test_coips(self):
        self.assertTrue(os.path.isfile('coips.xml'))

    def test_openssl(self):
        self.assertTrue(os.path.isfile('openssl.cnf'))

    def test_Certificates(self):
        self.assertTrue(os.path.isfile('Certificates.crt'))
