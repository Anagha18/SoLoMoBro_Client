#include <jni.h>
#include <string>
#include <android/log.h>
#include <unistd.h>

//#include "clientcode.h"
#include "ClusterCreate.h"
#include "util.h"

JNIEnv *print_env;
jobject print_ob;
JavaVM *g_VM;

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_clientside_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_clientside_MainActivity_infiniteLoop(JNIEnv *env, jobject instance) {


    while(1) {
//        __android_log_print(ANDROID_LOG_DEBUG, "CPP WHILE", "HELLO");
        LOGD("hello from cpp while loop");
        sleep(1);
    }

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_clientside_MainActivity_connectSocket(JNIEnv *env, jobject instance, jstring ip_) {
    const char *ip = env->GetStringUTFChars(ip_, 0);

    print_env = env;
    print_ob = instance;
    env->GetJavaVM(&g_VM);

    // mainfunc(ip);

    runclient(ip, 8080);

    env->ReleaseStringUTFChars(ip_, ip);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_clientside_MainActivity_cppMainFunc(JNIEnv *env, jobject thiz) {

}