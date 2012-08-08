#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <dirent.h>

void destroy(JNIEnv * env, JavaVM * jvm);

struct list_jar {
    char* value;
    struct list_jar * next;
};

struct jvm_struct {
	JNIEnv * env;
	JavaVM * jvm;
};

typedef struct list_jar item_jar;

char* getjars(const char* targetdir);

struct jvm_struct * startjvm();
void connect_proactive(struct jvm_struct * jvmp);

