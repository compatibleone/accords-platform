from distutils.core import setup, Extension


module1 = Extension('pycompmanager', 
                     sources = ['pyaccordsSDK/pycompmanager/pycompmanager.c','pyaccordsSDK/pycompmanager/compmanager.c','pyaccordsSDK/pycompmanager/list.c','pyaccordsSDK/pycompmanager/utools.c'],
                     depends = ['pyaccordsSDK/pycompmanager/compmanager.h','pyaccordsSDK/pycompmanager/list.c'],
                   )
setup (name = 'pycompmanager',
        version = '1.0',
        description = 'This is the pycompmanager package',
        ext_modules = [module1])
