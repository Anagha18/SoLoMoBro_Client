//
// Created by karan on 14-03-2020.
//

#ifndef CLIENTSIDE_UTIL_H
#define CLIENTSIDE_UTIL_H

#include <jni.h>
#include <android/log.h>

extern JNIEnv *print_env;
extern jobject print_ob;
extern JavaVM *g_VM;
void log_output(const char *fmtstring, ...);


#define  LOG_TAG    "NATIVE_SIDE"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#endif //CLIENTSIDE_UTIL_H
