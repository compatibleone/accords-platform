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
	author = 'Hamid Medjahed',
	author_email = 'hmedjahed@prologue.fr',
	url = 'www.compatibleone.org',
        description = 'This is the pycompmanager package of pyaccordsSDK',
        ext_modules = [module1])
