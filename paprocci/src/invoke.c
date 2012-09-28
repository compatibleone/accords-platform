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
#include <proactive.h>
#include <paconfig.h>
#include <restrequest.h>
#include <restresponse.h>
#include <restheader.h>

#define PATH_SEPARATOR ';' 
#define KEY_CLASSPATH "-Djava.class.path="
#define JAVA_MODULE_PATH "./java/"
#define STARTER_CLASS "org/ow2/compatibleone/Starter"
#define CLASS_REST_REQUEST "org/ow2/compatibleone/exchangeobjects/RestRequest"
#define CLASS_REST_RESPONSE "org/ow2/compatibleone/exchangeobjects/RestResponse"
#define CLASS_REST_HEADER "org/ow2/compatibleone/exchangeobjects/RestHeader"
#define CLASS_BLOCK_REST_HEADER "org/ow2/compatibleone/exchangeobjects/RestHeaderBlock"
#define CLASS_PROCCI_CATEGORY "org/ow2/compatibleone/exchangeobjects/ProcciCategory"

struct jvm_struct * initialize_jvm_if_needed(struct jvm_struct ** jvmpp);
jobject create_java_rest_request_object(struct jvm_struct * jvmp, struct rest_request * rptr);
jobject create_java_rest_response_object(struct jvm_struct * jvmp, struct rest_response * rptr);
jobject create_java_rest_header_object(struct jvm_struct * jvmp, struct rest_header * rptr);
jobject create_java_rest_header_full_block_object(struct jvm_struct * jvmp, struct rest_header * first);
jobject create_java_procci_category_object(struct jvm_struct * jvmp, struct proactive * category);

void test_invoke(){}
void test_invoke1(){
	struct jvm_struct * jvmp = NULL;
	jvmp = initialize_jvm_if_needed(&jvmp);


	fprintf(stderr, "If you see this, is because Mauricio is testing... :)\n");

	fprintf(stderr, "Creating rest_request...\n");
	struct rest_request a;
	a.first = NULL;
	a.last = NULL;
	a.method = "methoddd";
	a.object = "objecttt";
	a.parameters = "parameterssss";
	a.version = "versionnnn";
	a.type = 333;
	a.body = "bodyyyy";
	a.host = "hostttt";

	create_java_rest_request_object(jvmp, &a);

	fprintf(stderr, "Creating rest_response...\n");
	struct rest_response b;
	b.first = NULL;
	b.last = NULL;
	b.status = 333;
	b.version = "versionnnn";
	b.message = "messageeee";
	b.type = 333;
	b.body = "bodyyyy";

	create_java_rest_response_object(jvmp, &b);

	fprintf(stderr, "Creating rest_header...\n");
	struct rest_header c1;
	c1.next = NULL;
	c1.previous = NULL;
	c1.parent = NULL;
	c1.name = "name1";
	c1.value = "value1";

	struct rest_header c2;
	c2.next = NULL;
	c2.previous = NULL;
	c2.parent = NULL;
	c2.name = "name2";
	c2.value = "value2";

	c1.next = &c2;
	c2.previous = &c1;

	create_java_rest_header_full_block_object(jvmp, &c1);


	fprintf(stderr, "Creating procci category...\n");
	struct proactive d;
	d.id = "idddd";

	create_java_procci_category_object(jvmp, &d);


	fprintf(stderr, "All right!\n");
	exit(0);


}

jobject create_java_procci_category_object(struct jvm_struct * jvmp, struct proactive * category){
	// Initializing JVM parameters...
	JNIEnv *env = jvmp->env;
	JavaVM *jvm = jvmp->jvm;
	jobject procci = jvmp->procci;

	if (category == NULL){
		fprintf(stderr, "Can't accept NULL parameters...\n");
		exit(-1);
	}

	jclass sclass = (*env)->FindClass(env, CLASS_PROCCI_CATEGORY); 
	if (sclass == 0){
		fprintf(stderr, "Can't find class " CLASS_PROCCI_CATEGORY "...\n");
		exit(-1);
	}

	// Looking for instantiation method.
	jmethodID initmeth = (*env)->GetMethodID(env, sclass, "<init>","(Ljava/lang/String;)V");

	if (initmeth == 0){
		fprintf(stderr, "Can't find " CLASS_PROCCI_CATEGORY " constructor method...\n");
		exit(-1);
	}

	// Preparing parameters to instantiate the class...
	jstring id = (*env)->NewStringUTF(env, (category->id?category->id:""));	
	
	// Instantiating the class... 
	jobject restobj = (*env)->NewObject(env, sclass, initmeth, id);

	if (restobj == 0){
		fprintf(stderr, "Can't instantiate class " CLASS_PROCCI_CATEGORY "...\n");
		exit(-1);
	}

	return restobj;

}

jobject create_java_rest_header_full_block_object(struct jvm_struct * jvmp, struct rest_header * first){
	jmethodID initmeth;
	jmethodID addmeth;
	jobject resthead;
	
	JNIEnv *env = jvmp->env;
	JavaVM *jvm = jvmp->jvm;
	jobject procci = jvmp->procci;

	jclass sclass = (*env)->FindClass(env, CLASS_BLOCK_REST_HEADER);
	if (sclass == 0){
		fprintf(stderr, "Can't find class" CLASS_BLOCK_REST_HEADER "...\n");
		exit(-1);
	}

	initmeth = (*env)->GetMethodID(env, sclass, "<init>","()V");

	if (initmeth == 0){
		fprintf(stderr, "Can't find " CLASS_BLOCK_REST_HEADER " constructor method...\n");
		exit(-1);
	}

	resthead = (*env)->NewObject(env, sclass, initmeth);

	if (resthead == 0){
		fprintf(stderr, "Can't instantiate class " CLASS_BLOCK_REST_HEADER "...\n");
		exit(-1);
	}

	addmeth = (*env)->GetMethodID(env, sclass, "add","(Ljava/lang/String;Ljava/lang/String;)V");

	if (addmeth == 0){
		fprintf(stderr, "Can't find method to add...\n");
		exit(-1);
	}

	while(first != NULL){
		//fprintf(stderr, "Going through elements to add them %s...\n", first->name);
		jstring namestr = (*env)->NewStringUTF(env, first->name);
		jstring valuestr = (*env)->NewStringUTF(env, first->value);
		(*env)->CallVoidMethod(env, resthead, addmeth, namestr, valuestr);

		first = first->next;
	}

	return resthead;
}


jobject create_java_rest_response_object(struct jvm_struct * jvmp, struct rest_response * rptr){
	// Initializing JVM parameters...
	JNIEnv *env = jvmp->env;
	JavaVM *jvm = jvmp->jvm;
	jobject procci = jvmp->procci;

	if (rptr == NULL){
		fprintf(stderr, "Can't accept NULL parameters...\n");
		exit(-1);
	}

	jclass sclass = (*env)->FindClass(env, CLASS_REST_RESPONSE); 
	if (sclass == 0){
		fprintf(stderr, "Can't find class " CLASS_REST_RESPONSE "...\n");
		exit(-1);
	}

	// Looking for instantiation method.
	jmethodID initmeth = (*env)->GetMethodID(env, sclass, "<init>","(Lorg/ow2/compatibleone/exchangeobjects/RestHeader;Lorg/ow2/compatibleone/exchangeobjects/RestHeader;ILjava/lang/String;Ljava/lang/String;ILjava/lang/String;)V");

	if (initmeth == 0){
		fprintf(stderr, "Can't find " CLASS_REST_RESPONSE " constructor method...\n");
		exit(-1);
	}

	// Preparing parameters to instantiate the class...
	jobject first = create_java_rest_header_object(jvmp, NULL);
	jobject last = create_java_rest_header_object(jvmp, NULL);
	jint status = (jint) rptr->status;
	jstring version = (*env)->NewStringUTF(env, (rptr->version?rptr->version:""));	
	jstring message = (*env)->NewStringUTF(env, (rptr->message?rptr->message:""));	
	jint type = (jint) rptr->type;
	jstring body = (*env)->NewStringUTF(env, (rptr->body?rptr->body:""));	
	
	// Instantiating the class... 
	jobject restobj = (*env)->NewObject(env, sclass, initmeth, first, last, status, version, message, type, body);

	if (restobj == 0){
		fprintf(stderr, "Can't instantiate class " CLASS_REST_RESPONSE "...\n");
		exit(-1);
	}

	return restobj;

}

jobject create_java_rest_request_object(struct jvm_struct * jvmp, struct rest_request * rptr){
	// Initializing JVM parameters...
	JNIEnv *env = jvmp->env;
	JavaVM *jvm = jvmp->jvm;
	jobject procci = jvmp->procci;

	if (rptr == NULL){
		fprintf(stderr, "Can't accept NULL parameters...\n");
		exit(-1);
	}

	jclass sclass = (*env)->FindClass(env, CLASS_REST_REQUEST); 
	if (sclass == 0){
		fprintf(stderr, "Can't find class " CLASS_REST_REQUEST "...\n");
		exit(-1);
	}

	// Looking for instantiation method.
	jmethodID initmeth = (*env)->GetMethodID(env, sclass, "<init>","(Lorg/ow2/compatibleone/exchangeobjects/RestHeaderBlock;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;)V");

	if (initmeth == 0){
		fprintf(stderr, "Can't find " CLASS_REST_REQUEST " constructor method...\n");
		exit(-1);
	}

	// Preparing parameters to instantiate the class...
	//jobject first = create_java_rest_header_object(jvmp, NULL);
	//jobject last = create_java_rest_header_object(jvmp, NULL);
	jobject all = create_java_rest_header_full_block_object(jvmp, (rptr->first?rptr->first:NULL));
	jstring method = (*env)->NewStringUTF(env, (rptr->method?rptr->method:""));	
	jstring object = (*env)->NewStringUTF(env, (rptr->object?rptr->object:""));	
	jstring parameters = (*env)->NewStringUTF(env, (rptr->parameters?rptr->parameters:""));	
	jstring version = (*env)->NewStringUTF(env, (rptr->version?rptr->version:""));	
	jint type = (jint) rptr->type;
	jstring body = (*env)->NewStringUTF(env, (rptr->body?rptr->body:""));	
	jstring host = (*env)->NewStringUTF(env, (rptr->host?rptr->host:""));	
	
	// Instantiating the class... 
	jobject restobj = (*env)->NewObject(env, sclass, initmeth, all, method, object, parameters, version, type, body, host);

	if (restobj == 0){
		fprintf(stderr, "Can't instantiate class " CLASS_REST_REQUEST "...\n");
		exit(-1);
	}

	return restobj;

}


jobject create_java_rest_header_object(struct jvm_struct * jvmp, struct rest_header * rptr){
	jmethodID initmeth;
	jobject resthead;
	
	JNIEnv *env = jvmp->env;
	JavaVM *jvm = jvmp->jvm;
	jobject procci = jvmp->procci;

	jclass sclass = (*env)->FindClass(env, CLASS_REST_HEADER);
	if (sclass == 0){
		fprintf(stderr, "Can't find class " CLASS_REST_HEADER "...\n");
		exit(-1);
	}

	initmeth = (*env)->GetMethodID(env, sclass, "<init>","()V");

	if (initmeth == 0){
		fprintf(stderr, "Can't find " CLASS_REST_HEADER " constructor method...\n");
		exit(-1);
	}

	resthead = (*env)->NewObject(env, sclass, initmeth);

	if (resthead == 0){
		fprintf(stderr, "Can't instantiate class " CLASS_REST_HEADER "...\n");
		exit(-1);
	}

	return resthead;
}


/**
 * Get a list of .jar files to add them to the CLASSPATH for the VM to use them. 
 * DO NOT MODIFY THIS FUNCTION.
 */
char* getjars(const char* targetdir){
	DIR *dir;
	struct dirent *ent;
	char* buffer;
	item_jar * curr, * head;
	fprintf(stderr, "Looking for .jars in '%s'...\n", targetdir);
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
		fprintf(stderr, "Found: %s\n", curr->value);
		counter+=strlen(curr->value);
		curr = curr->next;
	}
	counter++;
	buffer = (char*) malloc(counter);
	buffer[0]='\0';

	curr = head;
	while(curr) {
		fprintf(stderr, "Using: %s\n", curr->value);
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
	jclass cls;
	jmethodID methodid;

	char* jarslist1 = getjars(JAVA_MODULE_PATH "lib/scheduling/");
	char* jarslist2 = getjars(JAVA_MODULE_PATH "lib/");
	char* jarslist3 = getjars(JAVA_MODULE_PATH "dist/");
	char* jarslist = (char*) malloc (strlen(jarslist1) + strlen(jarslist2) + strlen(jarslist3) + 1);
	jarslist[0] = '\0';
	//strcat(classpt, KEY_CLASSPATH);
	strcat(jarslist, jarslist1);
	strcat(jarslist, jarslist2);
	strcat(jarslist, jarslist3);
	free(jarslist1);
	free(jarslist2);
	free(jarslist3);
	fprintf(stderr, "Jars' list: '%s'\n", jarslist);
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
 * DO NOT MODIFY THIS FUNCTION.
 */
char * call_java_procci2(struct jvm_struct * jvmp, char * mname, char * msign, jobject request,  jobject response, jobject constr, jobjectArray margs){
	jstring jstr1;
	jstring jstr2;
	JNIEnv *env = jvmp->env;
	JavaVM *jvm = jvmp->jvm;
	jobject procci = jvmp->procci;

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

	jobject result = (*env)->CallObjectMethod(env, procci, methodid, request, response, constr, margs);
	fprintf(stderr, "Looking for exceptions...\n");
	if ((*env)->ExceptionOccurred(env)) {
		(*env)->ExceptionDescribe(env);
		return NULL;
	}else{
		fprintf(stderr, "Method '%s' executed.\n", mname);
		const char *str = (*env)->GetStringUTFChars(env,result,0);
		char * ret = (char*)malloc(strlen(str)+1);
		strcpy(ret, str);
		(*env)->ReleaseStringUTFChars(env, result, str);
		return ret;
	}
}


/** 
 * Make a call to the java layer.  
 * @param mname defines the name of the method to invoke
 * @param msign defines signature of the method to invoke
 * @param margs defines the concrete arguments to invoke the method
 * @return the json String of the result of the call.
 * DO NOT MODIFY THIS FUNCTION.
 */
char * call_java_procci(struct jvm_struct * jvmp, char * mname, char * msign, jobjectArray margs){
	jstring jstr1;
	jstring jstr2;
	JNIEnv *env = jvmp->env;
	JavaVM *jvm = jvmp->jvm;
	jobject procci = jvmp->procci;

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

	jobject result = (*env)->CallObjectMethod(env, procci, methodid, margs);
	fprintf(stderr, "Looking for exceptions...\n");
	if ((*env)->ExceptionOccurred(env)) {
		(*env)->ExceptionDescribe(env);
		return NULL;
	}else{
		fprintf(stderr, "Method '%s' executed.\n", mname);
		const char *str = (*env)->GetStringUTFChars(env,result,0);
		char * ret = (char*)malloc(strlen(str)+1);
		strcpy(ret, str);
		(*env)->ReleaseStringUTFChars(env, result, str);
		return ret;
	}
}

/**** ADJUST THE FOLLOWING CODE ACCORDING TO YOUR NEEDS ****/


/**
 * Initial connection to the cloud service provider.
 * It uses the Starter class to start up a procci connected to the provider.
 * MODIFY THIS FUNCTION ACCORDING TO YOUR NEEDS.
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

	// **** START MODIFYING HERE ***
	// Modify this section according to the arguments required for the 
	// connection to the cloud services provider.
	// For this case the arguments used are: user demo
	jclass sclass = (*env)->FindClass(env, "java/lang/String");
	jobjectArray margs = (*env)->NewObjectArray(env, 2, sclass, NULL); // Using 2 arguments.

	jstring jstr1 = (*env)->NewStringUTF(env, args[0]); // Creating argument 0.
	jstring jstr2 = (*env)->NewStringUTF(env, args[1]); // Creating argument 1.

	(*env)->SetObjectArrayElement(env, margs,0,jstr1); // Setting up argument 0.
	(*env)->SetObjectArrayElement(env, margs,1,jstr2); // Setting up argument 1.

	// **** STOP MODIFYING HERE ***

	jobject result; 
	result = (*env)->CallStaticObjectMethod(env, cls, methodid, margs);

	if ((*env)->ExceptionOccurred(env)) {
		fprintf(stderr, "Exception got when trying to connect to provider...\n");
		(*env)->ExceptionDescribe(env);
	}

	if (result == NULL) {
	    fprintf(stderr, "Could not get a result.\n");
	}
	jvmp->procci = result;
	fprintf(stderr, "Connected to cloud services provider...\n");

}

/**
 * Initialize the procci and connects it to the provider if needed. 
 * MODIFY THIS FUNCTION ACCORDING TO YOUR NEEDS.
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
 * MODIFY THIS FUNCTION ACCORDING TO YOUR NEEDS.
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
 * MODIFY THIS FUNCTION ACCORDING TO YOUR NEEDS.
 */
char * start_server(struct pa_config * config, struct jvm_struct ** jvmpp, struct proactive * constr) {

	jstring jstr1;
	jstring jstr2;
	jstring jstr3;
	
	struct jvm_struct * jvmp = initialize_provider_if_needed(config, jvmpp);

	JNIEnv *env = jvmp->env;
	JavaVM *jvm = jvmp->jvm;
	jobject procci = jvmp->procci;

	// The Java method to call is 'String start_server(Object[] args)'.
	// We prepare the arguments to invoke it. 
	jclass sclass = (*env)->FindClass(env, "java/lang/String");
	jobjectArray margs = (*env)->NewObjectArray(env, 4, sclass, NULL);

	jstr1 = (*env)->NewStringUTF(env, constr->image);	// Set argument. 
	//jstr2 = (*env)->NewStringUTF(env, "demo");
	//jstr3 = (*env)->NewStringUTF(env, "demo");

	(*env)->SetObjectArrayElement(env, margs,0,jstr1);	// Put argument into the array.
	//(*env)->SetObjectArrayElement(env, methodargs,1,jstr2);
	//(*env)->SetObjectArrayElement(env, methodargs,1,jstr3);

	return call_java_procci(jvmp, "start_server", "([Ljava/lang/Object;)Ljava/lang/String;", margs);
}

/**
 * Stop a VM.
 * MODIFY THIS FUNCTION ACCORDING TO YOUR NEEDS.
 */
char * stop_server(struct pa_config * config, struct jvm_struct ** jvmpp, struct rest_request * request,  struct rest_response * response, struct proactive * constr) {

	jstring jstr1;

	struct jvm_struct * jvmp = initialize_provider_if_needed(config, jvmpp);

	JNIEnv *env = jvmp->env;
	JavaVM *jvm = jvmp->jvm;
	jobject procci = jvmp->procci;

	jclass sclass = (*env)->FindClass(env, "java/lang/String");
	jobjectArray margs = (*env)->NewObjectArray(env, 1, sclass, NULL);

	printf("1");
	jstr1 = (*env)->NewStringUTF(env, constr->number);

	printf("2");
	(*env)->SetObjectArrayElement(env, margs,0,jstr1);

	printf("3");
	jobject requesto =  create_java_rest_request_object(jvmp, request);
	printf("4");
	return call_java_procci2(jvmp, "stop_server", "(Lorg/ow2/compatibleone/exchangeobjects/RestRequest;Lorg/ow2/compatibleone/exchangeobjects/RestResponse;Lorg/ow2/compatibleone/exchangeobjects/ProcciCategory;[Ljava/lang/Object;)Ljava/lang/String;", requesto, NULL, NULL,  margs);
 

}


