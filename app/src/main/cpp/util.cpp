//
// Created by karan on 14-03-2020.
//

#include <cstdlib>
#include "util.h"


void log_output(const char *fmtstring, ...)
{
    char *s = (char *) malloc(100 * sizeof(char));
    va_list argptr;
    va_start(argptr, fmtstring);
    vsprintf(s, fmtstring, argptr);
    va_end(argptr);

    LOGD("%s", s);

    LOGD("g_VM: %p\n", g_VM);
    g_VM->GetEnv((void **)&print_env, JNI_VERSION_1_6);
    g_VM->AttachCurrentThread(&print_env, NULL);
    LOGD("print_env: %p\n", print_env);
    print_env->ExceptionClear();
    jclass activityClass=print_env->FindClass("com/example/clientside/MainActivity");
    jmethodID methodId=print_env->GetStaticMethodID(activityClass, "log_output", "(Ljava/lang/String;)V");

    jstring st = print_env->NewStringUTF(s);
    print_env->CallStaticVoidMethod(activityClass, methodId, st);


    free(s);
}