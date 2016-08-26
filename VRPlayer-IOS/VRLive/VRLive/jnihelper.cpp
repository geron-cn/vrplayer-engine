#include <jni.h>
#include <stddef.h>
#include <pthread.h>
#include "3d/Scene.h"
#include "3d/Camera.h"
#include "3d/EventMgr.h"
#include <android/asset_manager_jni.h>

#include "FileUtils/FileUtils.h"
#include "FileUtils/FileSystem.h"
#include "3d/StringTextureUtil.h"
#include "3d/Label.h"
#include "3d/Action.h"
#include "3d/Texture.h"
#include "3d/MenuItem.h"
#include "3d/DefaultMenuItem.h"
#include "3d/Preference.h"
#include "jnihelper.h"
#include <random>

static vrlive::Scene* s_scene = NULL;
static JNIEnv* s_env = nullptr;

vrlive::Preference* preference = nullptr;

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
Java_com_vrlive_vrlib_common_JNIHelper_initRenderScene(JNIEnv *env, jobject instance, jstring configPath) {
    // TODO
    Java_com_vrlive_vrlib_common_JNIHelper_initClearRender(nullptr, nullptr);

    pthread_t thisthread = pthread_self();
    LOG("store env 1 pthread_self %p = %ld", env, thisthread);
    pthread_setspecific(g_key, env);
    s_scene = vrlive::Scene::create();

    const char *str;
    str = env->GetStringUTFChars(configPath, 0); 
    preference = vrlive::Preference::loadPreference(str, s_scene);
    env->ReleaseStringUTFChars(configPath, str);
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
    if(s_scene != nullptr)
    {
        s_scene->release();
        s_scene = nullptr;
    }

    if(preference != nullptr)
    {
        delete preference;
        preference = nullptr;
    }
}


JNIEXPORT void JNICALL Java_com_vrlive_vrlib_common_JNIHelper_setRenderMenuShow
  (JNIEnv *, jclass, jboolean ishow)
  {
      if(!s_scene)
        return;
      auto menus = s_scene->getDefMenuItem();
      menus->showPlayerMenu((bool)ishow);
  }

  JNIEXPORT void JNICALL Java_com_vrlive_vrlib_common_JNIHelper_setRenderCustomMenuShow
  (JNIEnv *, jclass, jboolean ishow)
  {
      if(!s_scene)
        return;
      auto menus = s_scene->getDefMenuItem();
      menus->setCustomMenuShow((bool)ishow);
  }

JNIEXPORT void JNICALL Java_com_vrlive_vrlib_common_JNIHelper_set2DCameraRotaion
  (JNIEnv *, jclass, jfloat rotaion)
  {
      vrlive::Label::rotationZ(rotaion);
  }


JNIEXPORT void JNICALL
Java_com_vrlive_vrlib_common_JNIHelper_setAssetManager(JNIEnv* env, jclass cls, jobject assetManager)
{
    vrlive::FileUtils::__assetManager = AAssetManager_fromJava( env, assetManager );
    vrlive::FileSystem::__assetManager = vrlive::FileUtils::__assetManager;
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

JNIEXPORT void JNICALL Java_com_vrlive_vrlib_common_JNIHelper_createMenuItem
  (JNIEnv *env, jclass, jstring name, jstring path, jfloat x, jfloat y, jfloat w, jfloat h)
  {
       vrlive::Scene* scene = s_scene;
       if(!scene)
            return;
       const char *pathStr;
       pathStr = env->GetStringUTFChars(path, 0); 
       auto menu = vrlive::MenuItem::create(pathStr, w, h);
       const char* nameStr;
       nameStr = env->GetStringUTFChars(name, 0);
       menu->setName(nameStr);
       vrlive::Vector3 pos(x, y, -20.f);
       menu->setTranslation(pos);
       scene->getDefMenuItem()->addChild(menu);
       menu->release();
       env->ReleaseStringUTFChars(path, pathStr);
       env->ReleaseStringUTFChars(name, nameStr);
  }

JNIEXPORT void JNICALL Java_com_vrlive_vrlib_common_JNIHelper_removeMenuItem
  (JNIEnv *env, jclass, jstring name)
  {
       vrlive::Scene* scene = s_scene;
       const char* nameStr;
       nameStr = env->GetStringUTFChars(name, 0);
       scene->getDefMenuItem()->removeChild(nameStr);
       env->ReleaseStringUTFChars(name, nameStr);
  }


  JNIEXPORT void JNICALL Java_com_vrlive_vrlib_common_JNIHelper_setCursorShow
  (JNIEnv *env, jclass, jboolean visible)
  {
      if(s_scene)
      {
          s_scene->setCursorVisible(visible);
      }
  }


JNIEXPORT void JNICALL Java_com_vrlive_vrlib_common_JNIHelper_removeLabel
  (JNIEnv *env, jclass, jstring name)
  {
       vrlive::Scene* scene = s_scene;
       const char* nameStr;
       nameStr = env->GetStringUTFChars(name, 0);
       scene->removeChild(nameStr);
       env->ReleaseStringUTFChars(name, nameStr);
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

    //create action
    vrlive::Vector3 startP, endP;
    translatePoint(startX, startY, scene, startP);
    translatePoint(targetX, targetY, scene, endP);
    auto moveaction = vrlive::MoveLineAction::create(startP, endP, duration);
    auto removeaction = vrlive::RemoveSelfAction::create(0.1); //delay 0.1 to remove label after move acton
    std::vector<vrlive::Action*> actions;
    actions.push_back(moveaction);
    actions.push_back(removeaction);
    auto actionsq = vrlive::SequnceAction::create(actions);
    //begin
    label->runAction(actionsq);
    scene->addChild(label);
    label->release();
    actionsq->release();
}


JNIEXPORT void JNICALL Java_com_vrlive_vrlib_common_JNIHelper_sendSequnceFrames
        (JNIEnv* env, jclass , jstring framesDir, jint startIndex, jint frameCount,
        jfloat startX, jfloat startY, jfloat targetX, jfloat targetY, jint duration)
{
     vrlive::Scene* scene = s_scene;
     const char *str;
     str = env->GetStringUTFChars(framesDir, 0); 
     std::string baseDir(str);
     auto strl = baseDir.length();
     if( strl != 0 && baseDir[strl-1] != '/')
        baseDir += "/";
     char pathOne[268];
     sprintf(pathOne, "%s%d%s", baseDir.c_str(), startIndex, ".png");
     auto label = vrlive::Label::createWithTexture(pathOne);
       //create action
     vrlive::Vector3 startP, endP;
     translatePoint(startX, startY, scene, startP);
     translatePoint(targetX, targetY, scene, endP);
     auto moveaction = vrlive::MoveLineAction::create(startP, endP, duration);
     auto removeaction = vrlive::RemoveSelfAction::create(0.1f); //delay 0.1 to remove label after move acton
     std::vector<vrlive::Action*> actions;
     actions.push_back(moveaction);
     actions.push_back(removeaction);
     auto actionsq = vrlive::SequnceAction::create(actions);
     auto frameaction = vrlive::FrameSequnceAction::create(baseDir, startIndex, frameCount, 0.1f, true);
     s_scene->addChild(label);
     label->runAction(frameaction);
     label->runAction(actionsq);
     frameaction->release();
     actionsq->release();
     label->release();
     env->ReleaseStringUTFChars(framesDir, str);
}

JNIEXPORT void JNICALL Java_com_vrlive_vrlib_common_JNIHelper_sendSpriteAnimate
  (JNIEnv* env, jclass, jstring spritePath, jfloat startX, jfloat startY,
            jfloat targetX, jfloat targetY, jint duration, jboolean fradeInOut)
{
     vrlive::Scene* scene = s_scene;
     const char *str;
     str = env->GetStringUTFChars(spritePath, 0); 
     auto label = vrlive::Label::createWithTexture(str);

      //create action
    vrlive::Vector3 startP, endP;
    translatePoint(startX, startY, scene, startP);
    translatePoint(targetX, targetY, scene, endP);
    auto moveaction = vrlive::MoveLineAction::create(startP, endP, duration);
    auto removeaction = vrlive::RemoveSelfAction::create(0.1); //delay 0.1 to remove label after move acton
    std::vector<vrlive::Action*> actions;
    actions.push_back(moveaction);
    actions.push_back(removeaction);
    auto actionsq = vrlive::SequnceAction::create(actions);
    std::random_device rd;
    auto scaleAction = vrlive::ScaleAction::create(.1f, 1.2f + (rd() % 100) / 100 * 0.5, duration);
   
    //begin
     if(fradeInOut)
     {
         auto fradeIn = vrlive::TintAction::create(vrlive::Vector4(1.f, 1.f, 1.f, 0.f), vrlive::Vector4(1.f, 1.f, 1.f, 1.f), duration * 0.5f);
         auto fradeOut = vrlive::TintAction::create(vrlive::Vector4(1.f, 1.f, 1.f, 1.f), vrlive::Vector4(1.f, 1.f, 1.f, 0.f), duration * 0.5f);
         std::vector<vrlive::Action*> fradeActions;
         fradeActions.push_back(fradeIn);
         fradeActions.push_back(fradeOut);
         auto fradesq = vrlive::SequnceAction::create(fradeActions);
         label->runAction(fradesq);
         fradesq->release();
     }
    label->runAction(scaleAction);
    label->runAction(actionsq);
    scene->addChild(label);
    label->release();
    scaleAction->release();
   // moveaction->release();
   // removeaction->release();
    actionsq->release();
    env->ReleaseStringUTFChars(spritePath, str);
}

JNIEXPORT void JNICALL Java_com_vrlive_vrlib_common_JNIHelper_loadNode
  (JNIEnv *env, jclass, jstring nodename)
{
     if(preference == nullptr)
         return;

    const char *str;
    str = env->GetStringUTFChars(nodename, 0);
    bool otherpropFile =  vrlive::FileSystem::fileExists(str);
    if(otherpropFile)
    {
        auto otherProp = vrlive::Preference::loadPreference(str, s_scene);
        delete otherProp;
    }
    else
        preference->loadNode(str);
    env->ReleaseStringUTFChars(nodename, str);
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
