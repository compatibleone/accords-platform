#include <invoke.h>

#define PATH_SEPARATOR ';' 
#define KEY_CLASSPATH "-Djava.class.path="
#define JAVA_MODULE_PATH "./java/"
#define STARTER_CLASS "org/ow2/compatibleone/Starter"

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
		fprintf(stderr, "Method %s found.\n", mname);
	}

	//jclass sclass = (*env)->FindClass(env, "java/lang/String");
	//jobjectArray methodargs = (*env)->NewObjectArray(env, 0, sclass, NULL);

	//(*env)->SetObjectArrayElement(env, methodargs,0,arg1);
	//(*env)->SetObjectArrayElement(env, methodargs,1,arg2);

	jobject result = (*env)->CallObjectMethod(env, procci, methodid, margs);
	fprintf(stderr, "Looking for exceptions...\n");
	if ((*env)->ExceptionOccurred(env)) {
		(*env)->ExceptionDescribe(env);
		return NULL;
	}else{
		fprintf(stderr, "Method %s executed.\n", mname);
		const char *str = (*env)->GetStringUTFChars(env,result,0);
		char * ret = (char*)malloc(strlen(str)+1);
		strcpy(ret, str);
		(*env)->ReleaseStringUTFChars(env, result, str);
		return ret;
	}
}

char * start_server(struct jvm_struct * jvmp, char* os) {

	jstring jstr1;
	jstring jstr2;
	jstring jstr3;
	JNIEnv *env = jvmp->env;
	JavaVM *jvm = jvmp->jvm;
	jobject procci = jvmp->procci;

	jclass sclass = (*env)->FindClass(env, "java/lang/String");
	jobjectArray margs = (*env)->NewObjectArray(env, 3, sclass, NULL);

	jstr1 = (*env)->NewStringUTF(env, os);
	//jstr2 = (*env)->NewStringUTF(env, "demo");
	//jstr3 = (*env)->NewStringUTF(env, "demo");

	(*env)->SetObjectArrayElement(env, margs,0,jstr1);
	//(*env)->SetObjectArrayElement(env, methodargs,1,jstr2);
	//(*env)->SetObjectArrayElement(env, methodargs,1,jstr3);

	return call_java_procci(jvmp, "start_server", "([Ljava/lang/Object;)Ljava/lang/String;", margs);
}

char * stop_server(struct jvm_struct * jvmp, char* id) {

	jstring jstr1;
	JNIEnv *env = jvmp->env;
	JavaVM *jvm = jvmp->jvm;
	jobject procci = jvmp->procci;

	jclass sclass = (*env)->FindClass(env, "java/lang/String");
	jobjectArray margs = (*env)->NewObjectArray(env, 1, sclass, NULL);

	jstr1 = (*env)->NewStringUTF(env, id);

	(*env)->SetObjectArrayElement(env, margs,0,jstr1);

	return call_java_procci(jvmp, "stop_server", "([Ljava/lang/Object;)Ljava/lang/String;", margs);
}

void connect_to_provider(struct jvm_struct * jvmp, char ** args) {
	// args: user demo
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

	jclass sclass = (*env)->FindClass(env, "java/lang/String");
	jobjectArray margs = (*env)->NewObjectArray(env, 2, sclass, NULL);

	jstring jstr1 = (*env)->NewStringUTF(env, args[0]);
	jstring jstr2 = (*env)->NewStringUTF(env, args[1]);

	(*env)->SetObjectArrayElement(env, margs,0,jstr1);
	(*env)->SetObjectArrayElement(env, margs,1,jstr2);

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
	fprintf(stderr, "Connected to ProActive...\n");

	
	/************TEST*****************/

	/*
	fprintf(stderr, "NOW EXPERIMENT\n");
	cls = (*env)->FindClass(env, "org/ow2/proactive/compatibleone/pajavaprocci/ProActiveProcci");
	if (cls == NULL) {
		fprintf(stderr, "Problem while finding the class ProActiveProcci...\n");
		destroy_jvm(env, jvm);
	}else{
		fprintf(stderr, "Class found.\n");
	}

	methodid = (*env)->GetMethodID(env, cls, "a", "([Ljava/lang/Object;)Ljava/lang/String;");
	if (methodid == NULL) {
		fprintf(stderr, "Could not find the method a.\n");
		destroy_jvm(env, jvm);
	}

	jclass sclass = (*env)->FindClass(env, "java/lang/String");
	jobjectArray arr = (*env)->NewObjectArray(env, 2, sclass, NULL);

	//To populate the array, use SetObjectArrayElement() to put a String
	(*env)->SetObjectArrayElement(env, arr,0,jstr1);
	(*env)->SetObjectArrayElement(env, arr,1,jstr2);


	fprintf(stderr, "Method a found....\n");
	result = (*env)->CallObjectMethod(env, result, methodid, arr);
	fprintf(stderr, "Method a executed....\n");
	//const char *str= (*env)->GetStringUTFChars(env,result,0);
	
	//printf("Result: %s\n", str);
	//need to release this string when done with it in order to avoid memory leak
	//(*env)->ReleaseStringUTFChars(env, result, str);

	//destroy_jvm(env, jvm);
	*/
}

struct jvm_struct * startjvm() {
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


