#include <jni.h>
#include <stddef.h>
#include <pthread.h>
#include "3d/Scene.h"
#include "3d/Camera.h"
#include "3d/EventMgr.h"
#include <android/asset_manager_jni.h>

#include "FileUtils/FileUtils.h"
#include "3d/StringTextureUtil.h"
#include "3d/Label.h"
#include "3d/Action.h"
#include "3d/Texture.h"
#include "jnihelper.h"

static vrlive::Scene* s_scene = NULL;
static JNIEnv* s_env = nullptr;
static JNIEnv* s_env_0 = nullptr;
static JNIEnv* s_env_1 = nullptr;

static pthread_key_t g_key;

static jmethodID sFindClassMethod;
static jobject    sClassLoader;
static JavaVM*   sJvm = nullptr;

JNIEXPORT void JNICALL
Java_com_vrlive_vrlib_common_JNIHelper_initClearEvent(JNIEnv *env, jobject instance) {

    // TODO
    vrlive::EventMgr::getInstance()->clearEvents();
}

JNIEXPORT void JNICALL
Java_com_vrlive_vrlib_common_JNIHelper_renderOndraw(JNIEnv *env, jobject instance,
                                                    jfloatArray matrix_) {
    jfloat *matrix = (env)->GetFloatArrayElements( matrix_, NULL);

    vrlive::Scene* scene = s_scene;
    // TODO
    if (scene)
    {
        vrlive::Camera* camera = scene->getCamera();
        float matrix_f[16];
        for(int i=0; i<16; i++)
        {
            matrix_f[i] = *(matrix +i);
        }
        camera->setViewMatrix(matrix_f);
        scene->draw();

       // LOGD("check events: %s ", _postedEvent.c_str());

    }
    (env)->ReleaseFloatArrayElements(matrix_, matrix, 0);
}

JNIEXPORT void JNICALL
Java_com_vrlive_vrlib_common_JNIHelper_renderOnChanged(JNIEnv *env, jobject instance, jint width, jint height) {

    vrlive::Scene* scene = s_scene;
    // TODO
    if (scene)
    {
        vrlive::Camera* camera = vrlive::Camera::createPerspective(60, (float)width/(float)height, 0.1f, 100.f);
        scene->setCamera(camera);
        scene->setWidth(width);
        scene->setHeight(height);
        camera->release();
    }
}

JNIEXPORT void JNICALL
Java_com_vrlive_vrlib_common_JNIHelper_initRenderScene(JNIEnv *env, jobject instance) {
    // TODO
    if(s_scene != nullptr)
        s_scene->release();

        pthread_t thisthread = pthread_self();
        LOG("store env 1 pthread_self %p = %ld", env, thisthread);
        pthread_setspecific(g_key, env);
        s_scene = vrlive::Scene::create();
}

JNIEXPORT jstring JNICALL
Java_com_vrlive_vrlib_common_JNIHelper_checkEvent(JNIEnv *env, jobject instance) {

    // TODO
    std::string eventStr = "";
    auto events = vrlive::EventMgr::getInstance()->getEvents();
    if (events.size() > 0)
        eventStr = events[0];
    return (env)->NewStringUTF(eventStr.c_str());
}

JNIEXPORT void JNICALL
Java_com_vrlive_vrlib_common_JNIHelper_initClearRender(JNIEnv *env, jobject instance) {
        s_scene->release();
        s_scene = NULL;
}

JNIEXPORT void JNICALL
Java_com_vrlive_vrlib_common_JNIHelper_setAssetManager(JNIEnv* env, jclass cls, jobject assetManager)
{
    vrlive::FileUtils::__assetManager = AAssetManager_fromJava( env, assetManager );
}


JNIEXPORT void JNICALL 
Java_com_vrlive_vrlib_common_JNIHelper_nativeInitBitmapDC(JNIEnv*  env, jclass cls, jint width, jint height, jbyteArray pixels)
{
    int size = width * height * 4;
    auto &bDC = vrlive::StringTextureUtil::bitmapDC;
    bDC._width = width;
    bDC._height = height;
    bDC._data = (unsigned char*)malloc(sizeof(unsigned char) * size);
    env->GetByteArrayRegion(pixels, 0, size, (jbyte*)bDC._data);
}

void translatePoint(const float& x, const float& y, vrlive::Scene* scene, vrlive::Vector3 &dst)
{
    dst.x = (x - .5f) * scene->getWidth();
    dst.y = (y - .5f) * scene->getHeight();
    dst.z = 0;
}

JNIEXPORT void JNICALL Java_com_vrlive_vrlib_common_JNIHelper_sendMessage
  (JNIEnv* env, jclass, jfloat startX, jfloat startY, jfloat targetX, jfloat targetY,
                                          jint width, jint height, jint duration, jbyteArray pixels)
{
    auto scene = s_scene;
    int sHeigth = 0, sWidth = 0;
    if( nullptr != scene)
    {
        sHeigth = scene->getHeight();
        sWidth = scene->getWidth();
    }
    if(sHeigth == 0 || sWidth == 0)
        return;

    // create label
    int size = width * height * 4;
    auto data = (unsigned char*)malloc(sizeof(unsigned char) * size);
    env->GetByteArrayRegion(pixels, 0, size, (jbyte*)data);
    auto tex = vrlive::Texture::create(vrlive::Texture::Format::RGBA, width, height, data);
    auto label = vrlive::Label::createWithTexture(tex);
    tex->release();
    free(data);
    if(label == nullptr)
    {
        LOG("created label failed");
    }
    else
    {
        LOG("created label success ");
    }

    //create action
    vrlive::Vector3 startP, endP;
    translatePoint(startX, startY, scene, startP);
    translatePoint(targetX, targetY, scene, endP);
    LOG(" %f, %f, %f, %f, %d, %d", startP.x, startP.y, endP.x, endP.y, width, height);
    auto moveaction = vrlive::MoveLineAction::create(startP, startP, duration);
     if(moveaction == nullptr)
    {
        LOG("created moveaction failed");
    }
    else
    {
        LOG("created moveaction success");
    }
    auto removeaction = vrlive::RemoveSelfAction::create(0.1); //delay 0.1 to remove label after move acton
     if(removeaction == nullptr)
    {
        LOG("created removeaction failed");
    }
    else
    {
        LOG("created removeaction success");
    } 
    std::vector<vrlive::Action*> actions;
    actions.push_back(moveaction);
    //actions.push_back(removeaction);
    auto actionsq = vrlive::SequnceAction::create(actions);
 if(actionsq == nullptr)
    {
        LOG("created actionsq failed");
    }
    else
    {
        LOG("created actionsq success");
    }
    //begin
    //label->runAction(actionsq);
    scene->addChild(label);
    // for test
    auto labeltest = vrlive::Label::create("test for label", "serif", 24, vrlive::Vector4(1.0f, 1.f, 1.f, 1.f));
    scene->addChild(labeltest);
    labeltest->release();

    label->release();
    actionsq->release();
}

JNIEnv* cacheEnv(JavaVM* jvm) {
    JNIEnv* _env = nullptr;
    // get jni environment
    jint ret = jvm->GetEnv((void**)&_env, JNI_VERSION_1_6);
    
    switch (ret) {
    case JNI_OK :
        // Success!
        pthread_setspecific(g_key, _env);
        return _env;
            
    case JNI_EDETACHED :
        // Thread not attached
        if (jvm->AttachCurrentThread(&_env, nullptr) < 0)
            {
                LOG("Failed to get the environment using AttachCurrentThread()");

                return nullptr;
            } else {
            // Success : Attached and obtained JNIEnv!
            pthread_setspecific(g_key, _env);
            return _env;
        }
            
    case JNI_EVERSION :
        // Cannot recover from this error
        LOG("JNI interface version 1.6 not supported");
    default :
        LOG("Failed to get the environment using GetEnv()");
        return nullptr;
    }
}

JNIEnv*  getEnv() {
    JNIEnv *_env = (JNIEnv *)pthread_getspecific(g_key);
    if (_env == nullptr)
    {
        LOG("pthread_self get nullptr env");
        _env = cacheEnv(sJvm);
    }
     pthread_t thisthread = pthread_self();
    LOG("pthread_self %p = %ld", _env, thisthread);
    return _env;
}

bool getMethodInfo_DefaultClassLoader(vrlive::JniMethodInfo &methodinfo,
                                                     const char *className,
                                                     const char *methodName,
                                                     const char *paramCode)
{
    if ((nullptr == className) ||
        (nullptr == methodName) ||
        (nullptr == paramCode)) {
        return false;
    }

    JNIEnv *env = getEnv();
    if (!env) {
        return false;
    }

    jclass classID = env->FindClass(className);
    if (! classID) {
        LOG("Failed to find class %s", className);
        env->ExceptionClear();
        return false;
    }

    jmethodID methodID = env->GetMethodID(classID, methodName, paramCode);
    if (! methodID) {
        LOG("Failed to find method id of %s", methodName);
        env->ExceptionClear();
        return false;
    }

    methodinfo.classID = classID;
    methodinfo.env = env;
    methodinfo.methodID = methodID;
    return true;
}

JNIEXPORT void JNICALL Java_com_vrlive_vrlib_common_JNIHelper_init
  (JNIEnv *env, jclass clazz, jobject activityIns)
  {
     pthread_t thisthread = pthread_self();
     LOG("store env main pthread_self %p = %ld", env, thisthread);
    pthread_setspecific(g_key, env);

        vrlive::JniMethodInfo _getclassloaderMethod;
        if (!getMethodInfo_DefaultClassLoader(_getclassloaderMethod,
                                                            "android/content/Context",
                                                            "getClassLoader",
                                                            "()Ljava/lang/ClassLoader;")) {
            return ;
        }

        jobject _c = getEnv()->CallObjectMethod(activityIns, _getclassloaderMethod.methodID);

        if (nullptr == _c) {
            return ;
        }

        vrlive::JniMethodInfo _m;
        if (!getMethodInfo_DefaultClassLoader(_m,
                                                            "java/lang/ClassLoader",
                                                            "loadClass",
                                                            "(Ljava/lang/String;)Ljava/lang/Class;")) {
            return ;
        }

        sClassLoader = getEnv()->NewGlobalRef(_c);
        sFindClassMethod = _m.methodID;
        LOG("init loaders");
  }

void _detachCurrentThread(void* a) {
    sJvm->DetachCurrentThread();
}


JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void *reserved)
{
    sJvm = vm;
    pthread_t thisthread = pthread_self();
    LOG("setJavaVM(%p), pthread_self() = %ld", sJvm, thisthread);

    //load appdelegate here
    
    pthread_key_create(&g_key, _detachCurrentThread);

    LOG("load completed");
    return JNI_VERSION_1_6;
}

bool getStaticMethodInfo(vrlive::JniMethodInfo &methodinfo,
                              const char *className,
                              const char *methodName,
                              const char *paramCode)
{

    methodinfo.env = getEnv();
    pthread_t thisthread = pthread_self();
    LOG("env (%p), pthread_self() = %ld", getEnv(), thisthread);
    jstring _jstrClassName = getEnv()->NewStringUTF(className);
    methodinfo.classID = (jclass) getEnv()->CallObjectMethod(sClassLoader, sFindClassMethod, _jstrClassName);
    methodinfo.methodID = getEnv()->GetStaticMethodID(methodinfo.classID, methodName, paramCode);
    getEnv()->DeleteLocalRef(_jstrClassName);
}
