import os
import unittest
import tempfile
import subprocess 

from hamcrest import assert_that, is_

from testfixtures import LogCapture

# Example command ./codegen.py -o ./tmp/ system_test/cords.xml system_test/test_config.xml

_path_to_codegen = './codegen.py'

def check_call(testcase, call):
    error = None
    try:
        subprocess.check_output(call)
    except subprocess.CalledProcessError as e:
        error = e.output
    assert_that(error, is_(None))

class TestCodegen(unittest.TestCase):
    '''
    These tests are intended to be a loose check that parsing of a set of files behaves as expected.  For detailed testing of particular configurations,
    refer to the unit tests for the parsing functionality, or extend the system tests with particular cases
    '''
    
    @classmethod
    def _make_test_files(cls): 
        cls._output_dir = tempfile.mkdtemp()
        #cls._config_file = tempfile.NamedTemporaryFile('w+', delete = False)
        #cls._config_filename = cls._config_file.name
        
    @classmethod
    def _delete_test_files(cls):
        filelist = [f for f in os.listdir(cls._output_dir)]
        for f in filelist:
            os.remove(f)
        os.rmdir(cls._output_dir)
        
        #cls._config_file.close()
        #os.remove(cls._config_filename)
        
    @classmethod
    def setUpClass(cls):
        cls._make_test_files()
        
    @classmethod
    def tearDownClass(cls):
        cls._delete_test_files() 
        
    def _input_files(self, files):
        return [os.path.join('system_test', file) for file in files]
        
    def test_that_parse_a_set_of_valid_files_does_not_crash(self):
        #check_call(self, [_path_to_codegen, "-o", self._output_dir, self._config_file.name])
        files = ["cords.xml", 'backend.xml', 'test_config.xml']
        check_call(self, [_path_to_codegen, "-o", self._output_dir] + self._input_files(files))
        
        