from distutils.core import setup, Extension
 
module1 = Extension('pycompmanager', 
                     sources = ['pycompmanager.c','compmanager.c','list.c','utools.c'],
                     depends = ['compmanager.h','list.c'],
                   )
setup (name = 'pycompmanager',
        version = '1.0',
        description = 'This is the pycompmanager package',
        ext_modules = [module1])
