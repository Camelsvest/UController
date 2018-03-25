//
// Created by Sean Zhang on 2018/3/5.
//

#ifndef UCONTROLLER_NATIVE_LIB_H
#define UCONTROLLER_NATIVE_LIB_H

#include <jni.h>

extern "C" {

JNIEXPORT jstring JNICALL Java_cn_urmet_ucontroller_HRBBus_stringFromJNI(
        JNIEnv *env,
        jobject /* this */);

JNIEXPORT void JNICALL Java_cn_urmet_ucontroller_HRBBus_test(
        JNIEnv *env,
        jobject thiz,
        jshort s, jint i, jlong l, jfloat f, jdouble d, jchar c, jboolean z, jbyte b, jstring str, jobject obj1, jintArray intArr);

JNIEXPORT jboolean JNICALL Java_cn_urmet_ucontroller_HRBBus_init(JNIEnv *env, jobject thiz);
JNIEXPORT void JNICALL Java_cn_urmet_ucontroller_HRBBus_uninit();
JNIEXPORT jint JNICALL Java_cn_urmet_ucontroller_HRBBus_sendMsg(
        JNIEnv *env, jobject obj, jint node, jchar status, jbyte addr0, jbyte addr1, jbyte addr2);
JNIEXPORT jboolean JNICALL Java_cn_urmet_ucontroller_HRBBus_setNotifier(JNIEnv *env, jobject thiz, jobject notifier);
};


#endif //UCONTROLLER_NATIVE_LIB_H
