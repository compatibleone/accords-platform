import unittest

from hamcrest import assert_that, is_

import codegen_types.ctypes as ctypes

class TestCTypes(unittest.TestCase):
    
    def test_that_ctypes_from_platform_type_returns_char_ptr_for_string(self):
        assert_that(ctypes.from_platform_type('string'), is_('char *'))
        
    def test_that_ctypes_from_platform_type_returns_int_for_int(self):
        assert_that(ctypes.from_platform_type('int'), is_('int'))
        
    def test_the_ctypes_from_platform_type_raises_ValueError_if_unexpected_type_passed(self):
        self.assertRaises(ValueError, ctypes.from_platform_type, 'something odd')