PROJECTNAME = JcompMgr
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
	@echo Creating C library...
	gcc -o lib$(PROJECTNAME).so $(PROJECTNAME).c compmgr.c jutools.c jlist.c -shared -fpic -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux -I../../coxml -lcoxml 
clean:
	rm *.class
	rm *.so

