from distutils.core import setup, Extension
 
module1 = Extension('pycompmanager', 
                     sources = ['pycompmanager.c','compmanager.c','list.c','utools.c'],
                     depends = ['compmanager.h','list.c'],
                     include_dirs = ['../../coxml/src/'],
		     libraries = ['coxml'],
                     library_dirs = ['../../coxml/src/.libs/']
                   )
setup (name = 'pycompmanager',
        version = '1.0',
        description = 'This is the pycompmanager package',
        ext_modules = [module1])
