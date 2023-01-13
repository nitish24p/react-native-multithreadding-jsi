#include <jni.h>
#include "react-native-multithreadding-lite.h"

extern "C"
JNIEXPORT jint JNICALL
Java_com_multithreaddinglite_MultithreaddingLiteModule_nativeMultiply(JNIEnv *env, jclass type, jdouble a, jdouble b) {
    return multithreaddinglite::multiply(a, b);
}
