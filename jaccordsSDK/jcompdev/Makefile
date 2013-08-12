PROJECTNAME = JcompDev

EXTRA_DIST = ../jcompmgr/jlist.c ../../jaccords/jsrc/jctools.c ../../jaccords/jsrc/jtools.c ../../jaccords/jsrc/*Interface.c
 
LD_FLAGS =  -locci -lpubocci -lcoxml -lcorest -lcords -lcocci -lcocarrier -lcopabr 

LD_JFLAGS = $(JAVA_HOME)/jre/lib/i386/client/libjvm.so

LD_INCLUDES = -I../../occi/ -I../../cocarrier/ -I../../corest/ -I../../cocci/ -I../../cords/ -I../../copabr/ -I../../coxml/  -I ../../broker/ -I../../pubocci/ -I../../jaccords/jsrc/

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
	@echo Creating Java binary jaccords base class
	javac $(CURDIR)/../../jaccords/jsrc/*Base.java 
	@echo Creating Java binary crud and actions  base class
	javac $(CURDIR)/../../jaccords/*.java -cp $(CURDIR)/../../jaccords/jsrc/
	@echo Creating Java binary crud and actions class
	javac $(CURDIR)/../../jaccords/jsrc/*Int.java -cp $(CURDIR)/../../jaccords/:$(CURDIR)/../../jaccords/jsrc/
	javac $(CURDIR)/../../jaccords/jsrc/*Act.java -cp $(CURDIR)/../../jaccords/:$(CURDIR)/../../jaccords/jsrc/ 
compilec:
	gcc -o lib$(PROJECTNAME).so $(PROJECTNAME).c compdev.c $(EXTRA_DIST) -shared -fpic -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux $(LD_INCLUDES) $(LD_FLAGS) $(LD_JFLAGS)
clean:
	rm *.class
	rm *.so

