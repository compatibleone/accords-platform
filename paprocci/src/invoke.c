/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2012 by Oasis (INRIA Sophia Antipolis) and ActiveEon teams.     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */

#include <invoke.h>

#define PATH_SEPARATOR ';' 
#define KEY_CLASSPATH "-Djava.class.path="
#define STARTER_CLASS "org/ow2/compatibleone/Starter"

struct jvm_struct * initialize_jvm_if_needed(struct jvm_struct ** jvmpp);

void test_invoke(){}

/**
 * Get a list of .jar files to add them to the CLASSPATH for the VM to use them. 
 * DO NOT MODIFY THIS FUNCTION.
 */
char* getjars(const char* targetdir){
	DIR *dir;
	struct dirent *ent;
	char* buffer;
	item_jar * curr, * head;
	head = NULL;
	dir = opendir (targetdir);
	if (dir != NULL) { /* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			curr = (item_jar *)malloc(sizeof(item_jar));
			curr->value = (char*)malloc(strlen(targetdir)+strlen(ent->d_name) + 1 + 1);
			curr->value[0]='\0';
			sprintf(curr->value,"%s%s:", targetdir, ent->d_name);
			curr->next  = head;
			head = curr;
		}
		closedir (dir);
	} else { /* could not open directory */
		perror ("");
	}

	curr = head;
	int counter=0;
	while(curr) {
		counter+=strlen(curr->value);
		curr = curr->next;
	}
	counter++;
	buffer = (char*) malloc(counter);
	buffer[0]='\0';

	curr = head;
	while(curr) {
		strcat (buffer,curr->value);
		free(curr->value);
		head = curr;
		curr = curr->next;
		free(head);
	}
	return buffer;
}

/**
 * Start the JVM where the Java side of this procci will run.
 * DO NOT MODIFY THIS FUNCTION.
 */
struct jvm_struct * start_jvm() {
	jint res;
	JNIEnv *env;
	JavaVM *jvm;

	char * jarslist = getjars("/usr/share/java/accords/paprocci/");
	fprintf(stderr, "List of jars: '%s'\n", jarslist);
#ifdef JNI_VERSION_1_2
	JavaVMInitArgs vm_args;
	JavaVMOption options[2];

	char* classpathoption = (char*) malloc (strlen(KEY_CLASSPATH) + strlen(jarslist) + 1);
	classpathoption[0]='\0';
	strcat(classpathoption, KEY_CLASSPATH);
	strcat(classpathoption, jarslist);
	free(jarslist);
	//options[0].optionString = "-Djava.class.path=" user_classpath;
	options[0].optionString = classpathoption;
	options[1].optionString = "-verbose:jni";
	vm_args.version = 0x00010002;
	vm_args.options = options;
	vm_args.nOptions = 1;
	vm_args.ignoreUnrecognized = JNI_TRUE;
	/* Create the Java VM */
	res = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
#else
	JDK1_1InitArgs vm_args;
	char classpath[1024];
	vm_args.version = 0x00010001;
	JNI_GetDefaultJavaVMInitArgs(&vm_args);
	/* Append user_classpath to the default system class path */
	sprintf(classpath, "%s%c%s",
	vm_args.classpath, PATH_SEPARATOR, jarslist);
	free(jarslist);
	vm_args.classpath = classpath;
	/* Create the Java VM */
	res = JNI_CreateJavaVM(&jvm, &env, &vm_args);
#endif /* JNI_VERSION_1_2 */

	if (res < 0) {
		fprintf(stderr, "Can't create Java VM\n");
		exit(1);
	}else{
		fprintf(stderr, "JVM created.\n");
	}

	struct jvm_struct * ret = (struct jvm_struct *) malloc(sizeof(struct jvm_struct));
	ret->jvm = jvm;
	ret->env = env;
	ret->procci = NULL;
	return ret;
}
/**
 * Try to exit gracefully, calling first the exit() method of the Starter class
 * to clean the JVM. 
 * DO NOT MODIFY THIS FUNCTION.
 */
void destroy_jvm(JNIEnv * env, JavaVM * jvm){
	jclass cls;
	jmethodID methodid;
	// Find the class to interact with. 
	cls = (*env)->FindClass(env, STARTER_CLASS);
	if (cls == NULL) {
		fprintf(stderr, "Problem while finding the class...\n");
		exit(0);
	}else{
		fprintf(stderr, "Class found.\n");
	}

	methodid = (*env)->GetStaticMethodID(env, cls, "exit", "()V");

	if (methodid == NULL) {
		fprintf(stderr, "Could not find the method.\n");
	}

	(*env)->CallStaticVoidMethod(env, cls, methodid);

	fprintf(stderr, "Executed the method to exit the JVM...\n");

	fprintf(stderr, "Looking for exceptions...\n");
	if ((*env)->ExceptionOccurred(env)) {
		(*env)->ExceptionDescribe(env);
		exit(1);
	}
	fprintf(stderr, "Done.\n");
	//printf("Destroying JVM...\n");
	//(*jvm)->DestroyJavaVM(jvm);
	//fprintf(stderr, "Done.\n");
	exit(0);
}

/** 
 * Make a call to the java layer.  
 * @param mname defines the name of the method to invoke
 * @param msign defines signature of the method to invoke
 * @param margs defines the concrete arguments to invoke the method
 * @return the json String of the result of the call.
 */
char * call_java_procci(struct jvm_struct * jvmp, char * mname, char * msign, jobjectArray margs){


	JNIEnv *env = jvmp->env;
	JavaVM *jvm = jvmp->jvm;

	fprintf(stderr, "Executing %s...\n", mname);
	
	jclass cls = (*env)->FindClass(env, "org/ow2/compatibleone/Procci");
	if (cls == NULL) {
		fprintf(stderr, "Problem while finding the interface Procci...\n");
		destroy_jvm(env, jvm);
	}else{
		fprintf(stderr, "Class found.\n");
	}

	jmethodID methodid = (*env)->GetMethodID(env, cls, mname, msign);
	if (methodid == NULL) {
		fprintf(stderr, "Could not find the method %s.\n", mname);
		destroy_jvm(env, jvm);
	}else{
		fprintf(stderr, "Method '%s' found.\n", mname);
	}

	fprintf(stderr, "Calling method '%s'...\n", mname);
	jobject result = (*env)->CallObjectMethod(env, jvmp->procci, methodid, margs);
	fprintf(stderr, "Done.\n");
	fprintf(stderr, "Looking for exceptions...\n");
	char * ret;
	if ((*env)->ExceptionOccurred(env)) {
		(*env)->ExceptionDescribe(env);
		ret = NULL;	
	}else{
		fprintf(stderr, "Method '%s' executed.\n", mname);
		jboolean isCopy;
		const char *str = (*env)->GetStringUTFChars(env,result,&isCopy);
		if (isCopy == JNI_TRUE) {
			ret = (char*)malloc(strlen(str)+1);
			strcpy(ret, str);
			(*env)->ReleaseStringUTFChars(env, result, str);
		}else{
			fprintf(stderr, "Problem getting the jstring result of the method '%s'...\n",mname);
		}
	}


	fprintf(stderr, "MMM 21...\n");
      	(*env)->DeleteLocalRef( env, result ); 
	fprintf(stderr, "MMM 22...\n");
      	(*env)->DeleteLocalRef( env, cls ); 
	fprintf(stderr, "MMM 23...\n");

	return ret;
}

/**
 * Initial connection to the cloud service provider.
 * It uses the Starter class to start up a procci connected to the provider.
 */
void connect_to_provider(struct jvm_struct * jvmp, char ** args) {
	JNIEnv *env = jvmp->env;
	JavaVM *jvm = jvmp->jvm;

	fprintf(stderr, "Executing connect_to_provider...\n");

	jclass cls = (*env)->FindClass(env, STARTER_CLASS);
	if (cls == NULL) {
		fprintf(stderr, "Problem while finding the class...\n");
		destroy_jvm(env, jvm);
	}else{
		fprintf(stderr, "Class found.\n");
	}

	jmethodID methodid = (*env)->GetStaticMethodID(env, cls, "start", "([Ljava/lang/Object;)Lorg/ow2/compatibleone/Procci;");
	if (methodid == NULL) {
		fprintf(stderr, "Could not find the method start.\n");
		destroy_jvm(env, jvm);
	}

	// Modify this section according to the arguments required for the 
	// connection to the cloud services provider.
	// For this case the arguments used are: user demo
	jclass sclass = (*env)->FindClass(env, "java/lang/String");
	jobjectArray margs = (*env)->NewObjectArray(env, 2, sclass, NULL); // Using 2 arguments.

	jstring jstr1 = (*env)->NewStringUTF(env, args[0]); // Creating argument 0.
	jstring jstr2 = (*env)->NewStringUTF(env, args[1]); // Creating argument 1.

	(*env)->SetObjectArrayElement(env, margs,0,jstr1); // Setting up argument 0.
	(*env)->SetObjectArrayElement(env, margs,1,jstr2); // Setting up argument 1.


	jobject result; 
	result = (*env)->CallStaticObjectMethod(env, cls, methodid, margs);

	if ((*env)->ExceptionOccurred(env)) {
		fprintf(stderr, "Exception got when trying to connect to provider...\n");
		(*env)->ExceptionDescribe(env);
	}

	/* Create a global reference */
	jobject resultGlobal = (*env)->NewGlobalRef(env, result);
		  
	/* The local reference is no longer useful */
	(*env)->DeleteLocalRef(env, result);

	//fprintf(stderr, "MMM z1...\n");
      	//(*env)->DeleteLocalRef( env, result ); 

	fprintf(stderr, "MMM z2...\n");
      	(*env)->DeleteLocalRef( env, margs ); 
	fprintf(stderr, "MMM z3...\n");
      	(*env)->DeleteLocalRef( env, sclass ); 
	fprintf(stderr, "MMM z4...\n");
      	(*env)->DeleteLocalRef( env, jstr1 ); 
	fprintf(stderr, "MMM z5...\n");
      	(*env)->DeleteLocalRef( env, jstr2 ); 
	fprintf(stderr, "MMM z6...\n");

	//jvmp->procci = result;
	jvmp->procci = resultGlobal;
	fprintf(stderr, "Connected to cloud services provider...\n");
}

/**
 * Initialize the procci and connects it to the provider if needed. 
 */
struct jvm_struct * initialize_provider_if_needed(struct pa_config * config, struct jvm_struct ** jvmpp){
	if (*jvmpp==NULL){
		fprintf(stderr, "Initializing JVM and provider...\n");
		// start the JVM
		char ** args = (char**)malloc(2 * sizeof(char*));
		args[0] = config->user;
		args[1] = config->password;

		fprintf(stderr, "Using user='%s', demo='%s'...\n", args[0], args[1]);
		*jvmpp = start_jvm();
		connect_to_provider(*jvmpp, args);
		free(args);
		fprintf(stderr, "JVM and provider were initialized successfully.\n");
	}else{
		fprintf(stderr, "Provider initialized, skipping...\n");
	}
	return *jvmpp;
}

/**
 * Initialize the procci and connects it to the provider if needed. 
 */
struct jvm_struct * initialize_jvm_if_needed(struct jvm_struct ** jvmpp){
	if (*jvmpp==NULL){
		fprintf(stderr, "Initializing JVM (NOT provider)...\n");
		// start the JVM
		*jvmpp = start_jvm();
		fprintf(stderr, "JVM and was initialized successfully.\n");
	}else{
		fprintf(stderr, "Provider initialized, skipping...\n");
	}
	return *jvmpp;
}

/**
 * Start a VM.
 * @param config procci configuration structure.
 * @param jvmpp procci JVM structure.
 * @param constr constraints for this virtual machine. 
 * @return the resulting json object of the java call.
 */
char * start_server(struct pa_config * config, struct jvm_struct ** jvmpp, struct rest_request * request,  struct rest_response * response, struct proactive * constr) {

	jstring jstr1;
	//jstring jstr2;
	
	struct jvm_struct * jvmp = initialize_provider_if_needed(config, jvmpp);

	JNIEnv *env = jvmp->env;
	JavaVM *jvm = jvmp->jvm;
	jobject procci = jvmp->procci;

	// The Java method to call is 'String start_server(Object[] args)'.
	// We prepare the arguments to invoke it. 
	jclass sclass = (*env)->FindClass(env, "java/lang/String");
	//jobjectArray margs = (*env)->NewObjectArray(env, 5, sclass, NULL);
	jobjectArray margs = (*env)->NewObjectArray(env, 1, sclass, NULL);

	jstr1 = (*env)->NewStringUTF(env, constr->image);	// Set argument. 
	//jstr2 = (*env)->NewStringUTF(env, "2");			// Set argument. 

	(*env)->SetObjectArrayElement(env, margs,0,jstr1);	// Put argument into the array.
	//(*env)->SetObjectArrayElement(env, margs,1,jstr2);	// Put argument into the array.

	char* retur = call_java_procci(jvmp, "start_server", "([Ljava/lang/Object;)Ljava/lang/String;", margs);

	
	fprintf(stderr, "MMM a...\n");
      	(*env)->DeleteLocalRef( env, jstr1 ); 
	fprintf(stderr, "MMM b...\n");
      	(*env)->DeleteLocalRef( env, margs ); 
	fprintf(stderr, "MMM c...\n");
 	(*env)->DeleteLocalRef( env, sclass ); 
	fprintf(stderr, "MMM d...\n");
	

	return retur;
}

/**
 * Stop a VM.
 */
char * stop_server(struct pa_config * config, struct jvm_struct ** jvmpp, struct rest_request * request,  struct rest_response * response, struct proactive * constr) {

	jstring jstr1;

	struct jvm_struct * jvmp = initialize_provider_if_needed(config, jvmpp);

	JNIEnv *env = jvmp->env;
	JavaVM *jvm = jvmp->jvm;

	jclass sclass = (*env)->FindClass(env, "java/lang/String");
	jobjectArray margs = (*env)->NewObjectArray(env, 1, sclass, NULL);

	jstr1 = (*env)->NewStringUTF(env, constr->number);

	(*env)->SetObjectArrayElement(env, margs,0,jstr1);

	char* retur = call_java_procci(jvmp, "stop_server", "([Ljava/lang/Object;)Ljava/lang/String;", margs);

	fprintf(stderr, "MMM 1...\n");
      	(*env)->DeleteLocalRef( env, jstr1 ); 
	fprintf(stderr, "MMM 2...\n");
      	(*env)->DeleteLocalRef( env, margs ); 
	fprintf(stderr, "MMM 3...\n");
 	(*env)->DeleteLocalRef( env, sclass ); 
	fprintf(stderr, "MMM 4...\n");

	return retur;
}


