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

    print_env->ExceptionClear();
    jclass activityClass=print_env->GetObjectClass(print_ob);
    jmethodID methodId=print_env->GetMethodID(activityClass, "log_output", "(Ljava/lang/String;)V");

    jstring st = print_env->NewStringUTF(s);
    print_env->CallVoidMethod(print_ob, methodId, st);

    free(s);
}