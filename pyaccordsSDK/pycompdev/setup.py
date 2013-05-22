from distutils.core import setup, Extension
 
module1 = Extension('pycompdev', 
                     sources = ['pycompdev.c','list.c','../../pyaccords/pysrc/ctools.c','../../pyaccords/pysrc/pytools.c'],
                     depends = ['component.c','list.c','../../pyaccords/pysrc/listoccibuilder.h','../../pyaccords/pysrc/*','../../pyaccords/pygen'],
                     include_dirs=['../../occi/','../../cocarrier/','../../cocci/','../../coes/','../../command/','../../copabr/','../../cords/','../../corest/','../../coxml/','../../pubocci/','../../pyaccords/pysrc/','../pyaccords/pygen/'],
                     libraries=['occi','pubocci','coxml','corest','cords','cocci','cocarrier','copabr'],
                     library_dirs=['../../occi/.libs/','../../pubocci/.libs/','../../coxml/.libs/','../../corest/.libs/',
                                   '../../cords/.libs/','../../cocci/.libs/','../../cocarrier/.libs/','../../copabr/.libs/'
                                  ]
                   )
 
setup (name = 'pycompdev',
        version = '1.0',
	author = 'Hamid Medjahed',
	author_email = 'hmedjahed@prologue.fr',
	url = 'www.compatibleone.org',
        description = 'This is the pycompdev package of pyaccordsSDK',
        ext_modules = [module1])
