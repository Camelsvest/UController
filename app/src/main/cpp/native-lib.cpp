#include <string>
#include <malloc.h>
#include <android/log.h>
#include "hrbbus.h"

#include "debug_log.h"
#include "../jni"

JNIEXPORT jstring JNICALL
Java_cn_urmet_ucontroller_HRBBus_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    LOG_DEBUG("%s invoked",__FUNCTION__);
    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT void JNICALL
Java_cn_urmet_ucontroller_HRBBus_test(
        JNIEnv *env,
        jobject thiz,
        jshort s, jint i, jlong l, jfloat f, jdouble d, jchar c, jboolean z, jbyte b, jstring str, jobject obj1, jintArray intArr) {

    LOG_DEBUG("s=%hd, i=%d, l=%lld, f=%f, d=%lf, c=%c, z=%c, b=%d", s, i, l, f, d, c, z, b);
    const char *c_str = NULL;
    c_str = env->GetStringUTFChars(str, NULL);
    if (c_str == NULL)
    {
        return; // memory out
    }

    LOG_DEBUG("c_str: %s\n", (char*)c_str);

    env->ReleaseStringUTFChars(str, c_str);

    int len = env->GetArrayLength(intArr);
}

HRBBus BUS;
//JavaVM *g_javaVM = NULL;
//jint g_jniVersion;

void onNotifyStatus(unsigned short int status, unsigned short int dest_address, void *param) {
/*
    jclass clazz;
    JavaVM *javaVM;
    JNIEnv *env = NULL;

    if (param == NULL)
        return;

    javaVM = (JavaVM *)param;
    if (javaVM->GetEnv((void **)&env, g_jniVersion) != JNI_OK)
        return;
*/
}

JNIEXPORT jboolean JNICALL Java_cn_urmet_ucontroller_HRBBus_init(
        JNIEnv *env,
        jobject thiz)
{
    jint ret;

    LOG_DEBUG("HRBBus init is invoked in NATIVE");
/*
    env->GetJavaVM(&g_javaVM);
    LOG_DEBUG("Java VM pointer 0x%p", g_javaVM);

    g_jniVersion = env->GetVersion();

    ret = BUS.init(onNotifyStatus, g_javaVM);
    LOG_DEBUG("Get the return value for HRBBus init:%d.\n",ret);
*/
    ret = BUS.init();
    return 0;
}

JNIEXPORT void JNICALL Java_cn_urmet_ucontroller_HRBBus_uninit
        (JNIEnv *, jobject) {

    LOG_DEBUG("HRBBus uninit is invoked in NATIVE");
    BUS.uninit();
}

JNIEXPORT jint JNICALL Java_cn_urmet_ucontroller_HRBBus_sendMsg
        (JNIEnv *env, jobject obj, jint node, jshort status, jbyte addr0, jbyte addr1, jbyte addr2) {

    LOG_DEBUG("HRBBus sendMsg:%x %x %x %x",status, addr0, addr1, addr2);
    return BUS.sendMsg(node, status, addr0, addr1, addr2);
}

JNIEXPORT jboolean JNICALL Java_cn_urmet_ucontroller_HRBBus_setNotifier(JNIEnv *env, jobject thiz, jobject notifier) {
    return BUS.setNotifier(env, thiz, notifier);
}