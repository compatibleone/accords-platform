PROJECTNAME = JcompDev

EXTRA_DIST = ../jcompmgr/jlist.c ../../jaccords/jsrc/jctools.c ../../jaccords/jsrc/jtools.c

LD_FLAGS =  -locci -lpubocci -lcoxml -lcorest -lcords -lcocci -lcocarrier -lcopabr 

LD_JFLAGS = $(JAVA_HOME)/jre/lib/i386/client/libjvm.so

LD_INCLUDES = -I../../occi/src/ -I../../cocarrier/src/ -I../../corest/src/ -I../../cocci/src/ -I../../cords/src/ -I../../copabr/src/ -I../../coxml/src/  -I ../../broker/src -I../../pubocci/src/ -I../../jaccords/jsrc/

default:
	@echo jaccordsSDK $(PROJECTNAME)
	@echo Make sure to have defined the environment variable JAVA_HOME pointing to a JDK 1.6.
	@echo Now, execute all these items respecting this order:
	@echo .  make compilejava
	@echo .  make compilec
	@echo .  \(make clean\)
compilejava: 
	@echo Creating Java binary...
	javac $(PROJECTNAME).java
compilec:
	gcc -o lib$(PROJECTNAME).so $(PROJECTNAME).c compdev.c $(EXTRA_DIST) -shared -fpic -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux $(LD_INCLUDES) $(LD_FLAGS) $(LD_JFLAGS)
clean:
	rm *.class
	rm *.so

