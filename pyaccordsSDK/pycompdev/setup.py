from distutils.core import setup, Extension
 
module1 = Extension('pycompdev', 
                     sources = ['pycompdev.c','list.c','../../pyaccords/pysrc/ctools.c','../../pyaccords/pysrc/pytools.c'],
                     depends = ['component.c','list.c','../../pyaccords/pysrc/listoccibuilder.h','../../pyaccords/pysrc/*'],
                     include_dirs=['../../occi/src/','../../cocarrier/src/','../../cocci/src/','../../coes/src/','../../command/src/','../../copabr/src','../../cords/src/','../../corest/src','../../coxml/src/','../../pubocci/src/','../../pyaccords/pysrc/'],
                     libraries=['occi','pubocci','coxml','corest','cords','cocci','cocarrier','copabr'],
                     library_dirs=['../../occi/src/.libs/','../../pubocci/src/.libs/','../../coxml/src/.libs/','../../corest/src/.libs/',
                                   '../../cords/src/.libs/','../../cocci/src/.libs/','../../cocarrier/src/.libs/','../../copabr/src/.libs/'
                                  ]
                   )
 
setup (name = 'pycompdev',
        version = '1.0',
        description = 'This is the pycompdev package',
        ext_modules = [module1])
