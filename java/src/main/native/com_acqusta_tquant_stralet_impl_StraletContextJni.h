/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_acqusta_tquant_stralet_impl_StraletContextJni */

#ifndef _Included_com_acqusta_tquant_stralet_impl_StraletContextJni
#define _Included_com_acqusta_tquant_stralet_impl_StraletContextJni
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_acqusta_tquant_stralet_impl_StraletContextJni
 * Method:    getTradingDay
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_acqusta_tquant_stralet_impl_StraletContextJni_getTradingDay
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_acqusta_tquant_stralet_impl_StraletContextJni
 * Method:    getCurTime
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_acqusta_tquant_stralet_impl_StraletContextJni_getCurTime
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_acqusta_tquant_stralet_impl_StraletContextJni
 * Method:    postEvent
 * Signature: (JLjava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_com_acqusta_tquant_stralet_impl_StraletContextJni_postEvent
  (JNIEnv *, jclass, jlong, jstring, jlong);

/*
 * Class:     com_acqusta_tquant_stralet_impl_StraletContextJni
 * Method:    setTimer
 * Signature: (JJJJ)V
 */
JNIEXPORT void JNICALL Java_com_acqusta_tquant_stralet_impl_StraletContextJni_setTimer
  (JNIEnv *, jclass, jlong, jlong, jlong, jlong);

/*
 * Class:     com_acqusta_tquant_stralet_impl_StraletContextJni
 * Method:    killTimer
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_acqusta_tquant_stralet_impl_StraletContextJni_killTimer
  (JNIEnv *, jclass, jlong, jlong);

/*
 * Class:     com_acqusta_tquant_stralet_impl_StraletContextJni
 * Method:    getDataApi
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_acqusta_tquant_stralet_impl_StraletContextJni_getDataApi
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_acqusta_tquant_stralet_impl_StraletContextJni
 * Method:    getTradeApi
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_acqusta_tquant_stralet_impl_StraletContextJni_getTradeApi
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_acqusta_tquant_stralet_impl_StraletContextJni
 * Method:    log
 * Signature: (JILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_acqusta_tquant_stralet_impl_StraletContextJni_log
  (JNIEnv *, jclass, jlong, jint, jstring);

/*
 * Class:     com_acqusta_tquant_stralet_impl_StraletContextJni
 * Method:    getProperties
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_acqusta_tquant_stralet_impl_StraletContextJni_getProperties
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_acqusta_tquant_stralet_impl_StraletContextJni
 * Method:    getMode
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_acqusta_tquant_stralet_impl_StraletContextJni_getMode
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_acqusta_tquant_stralet_impl_StraletContextJni
 * Method:    stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_acqusta_tquant_stralet_impl_StraletContextJni_stop
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
