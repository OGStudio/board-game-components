FEATURE library.cpp/Impl
#define JNI_FUNC(FUNC_NAME) \
    JNIEXPORT void JNICALL Java_org_opengamestudio_omc05_library_ ## FUNC_NAME
#define JNI_FUNC_ARRAY(FUNC_NAME) \
    JNIEXPORT jobjectArray JNICALL Java_org_opengamestudio_omc05_library_ ## FUNC_NAME
#define JNI_ARG JNIEnv *env, jobject /* this */
