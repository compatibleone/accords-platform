# vim: set et sw=4 ts=4 ai:

import os
import subprocess

from utils import *

class TestBin():

    def test_presence(self):
        self.assertTrue(os.path.isfile('/usr/bin/' + self.bin))

    def test_runnning(self):
        # Check if it's explicitely disabled or enabled
        try:
            if self.do_not_test_running:
                self.assertTrue(True)
                return
        # If it's not explicitely disabled
        except AttributeError:
            self.assertEqual(subprocess.call(['/usr/bin/' + self.bin]), 0)
