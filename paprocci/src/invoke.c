#include <invoke.h>

#define PATH_SEPARATOR ';' /* define it to be ':' on Solaris */
#define KEY_CLASSPATH "-Djava.class.path="
#define JAVA_MODULE_PATH "./java/"

void destroy(JNIEnv * env, JavaVM * jvm){
	jclass cls;
	jmethodID methodid;
	// Find the class to interact with. 
	cls = (*env)->FindClass(env, "org/ow2/proactive/compatibleone/pajavaprocci/Starter");
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


void start_server(struct jvm_struct * jvmp) {
	jclass cls;
	jmethodID methodid;
	jstring jstr1;
	jstring jstr2;
	jclass stringClass;
	jobjectArray args;
	jobject procci;
	jobject result;
	JNIEnv *env;
	JavaVM *jvm;
	env = jvmp->env;
	jvm = jvmp->jvm;
	procci = jvmp->procci;

	fprintf(stderr, "Executing start_server...\n");
	
	cls = (*env)->FindClass(env, "org/ow2/proactive/compatibleone/pajavaprocci/ProActiveProcci");
	if (cls == NULL) {
		fprintf(stderr, "Problem while finding the class ProActiveProcci...\n");
		destroy(env, jvm);
	}else{
		fprintf(stderr, "Class found.\n");
	}

	methodid = (*env)->GetMethodID(env, cls, "listNodes", "()Ljava/lang/String;");
	if (methodid == NULL) {
		fprintf(stderr, "Could not find the method listNodes.\n");
		destroy(env, jvm);
	}

	result = (*env)->CallObjectMethod(env, procci, methodid);
	const char *str= (*env)->GetStringUTFChars(env,result,0);
	
	printf("Result: %s\n", str);
	//need to release this string when done with it in order to avoid memory leak
	(*env)->ReleaseStringUTFChars(env, result, str);

	/*
	destroy(env, jvm);
	*/
}

void connect_to_provider(struct jvm_struct * jvmp) {
	jclass cls;
	jmethodID methodid;
	jstring jstr1;
	jstring jstr2;
	jclass stringClass;
	jobjectArray args;
	JNIEnv *env;
	JavaVM *jvm;
	env = jvmp->env;
	jvm = jvmp->jvm;

	fprintf(stderr, "Executing connect_to_provider...\n");
	// Find the class to interact with. 
	//cls = (*env)->FindClass(env, "Prog");
	cls = (*env)->FindClass(env, "org/ow2/proactive/compatibleone/pajavaprocci/Starter");
	if (cls == NULL) {
		fprintf(stderr, "Problem while finding the class...\n");
		destroy(env, jvm);
	}else{
		fprintf(stderr, "Class found.\n");
	}
	//methodid = (*env)->GetStaticMethodID(env, cls, "setA", "(Ljava/lang/String;)V"); // Working.
	//methodid = (*env)->GetStaticMethodID(env, cls, "getA", "()Ljava/lang/String;");

	methodid = (*env)->GetStaticMethodID(env, cls, "start", "(Ljava/lang/String;Ljava/lang/String;)Lorg/ow2/proactive/compatibleone/pajavaprocci/ProActiveProcci;");
	if (methodid == NULL) {
		fprintf(stderr, "Could not find the method start.\n");
		destroy(env, jvm);
	}

	jstr1 = (*env)->NewStringUTF(env, "demo");
	if (jstr1 == NULL) {
		destroy(env, jvm);
	}

	jstr2 = (*env)->NewStringUTF(env, "demo");
	if (jstr2 == NULL) {
		destroy(env, jvm);
	}

	jobject result; 
	result = (*env)->CallStaticObjectMethod(env, cls, methodid, jstr1, jstr2);

	//result = (*env)->CallStaticObjectMethod(env, cls, methodid, NULL);

	if (result == NULL) {
	    fprintf(stderr, "Could not get a result.\n");
	}
	jvmp->procci = result;
	fprintf(stderr, "Connected to ProActive...\n");

	
	fprintf(stderr, "NOW EXPERIMENT\n");
	cls = (*env)->FindClass(env, "org/ow2/proactive/compatibleone/pajavaprocci/ProActiveProcci");
	if (cls == NULL) {
		fprintf(stderr, "Problem while finding the class ProActiveProcci...\n");
		destroy(env, jvm);
	}else{
		fprintf(stderr, "Class found.\n");
	}

	methodid = (*env)->GetMethodID(env, cls, "listNodes", "()Ljava/lang/String;");
	if (methodid == NULL) {
		fprintf(stderr, "Could not find the method listNodes.\n");
		destroy(env, jvm);
	}

	result = (*env)->CallObjectMethod(env, result, methodid, jstr1, jstr2);
	const char *str= (*env)->GetStringUTFChars(env,result,0);
	
	printf("Result: %s\n", str);
	//need to release this string when done with it in order to avoid memory leak
	(*env)->ReleaseStringUTFChars(env, result, str);

	/*
	destroy(env, jvm);
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


	//methodid = (*env)->GetStaticMethodID(env, cls, "main", "([Ljava/lang/String;)V");// Working. 
	//methodid = (*env)->GetStaticMethodID(env, cls, "setA", "(Ljava/lang/String;)V"); // Working.
	//methodid = (*env)->GetStaticMethodID(env, cls, "getA", "()Ljava/lang/String;");
	/*
	methodid = (*env)->GetStaticMethodID(env, cls, "start", "(Ljava/lang/String;Ljava/lang/String;)V");
	if (methodid == NULL) {
	fprintf(stderr, "Could not find the method.\n");
	destroy(env, jvm);
	}

	*/ 
	//jobject result; 
	//result = (*env)->CallStaticObjectMethod(env, cls, methodid, NULL);

	//if (result == NULL) {
	//    fprintf(stderr, "Could not get a result.\n");
	//}

	//const char *str= (*env)->GetStringUTFChars(env,result,0);
	//printf("Result: %s\n", str);
	//need to release this string when done with it in order to avoid memory leak
	//(*env)->ReleaseStringUTFChars(env, result, str);

	/*
	destroy(env, jvm);
	*/
	struct jvm_struct * ret = (struct jvm_struct *) malloc(sizeof(struct jvm_struct));
	ret->jvm = jvm;
	ret->env = env;
	ret->procci = NULL;
	return ret;
}


