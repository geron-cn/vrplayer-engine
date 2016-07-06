#include <math.h>
#include "Ray.h"
#include "DefaultMenuItem.h"
#include "Texture.h"
#include "Sprite3D.h"
#include "camera.h"
#include "MenuItem.h"
#include "Stream.h"
#include "Scene.h"
#include "DefaultMenuItem.inl"
#include "Stream.h"

vrlive::Stream* getBtnBackStream()
{
    char* buff = new char[rc1::pngVRBack_size];
    
    memcpy(buff, rc1::pngVRBack, rc1::pngVRBack_size);
    
    return vrlive::MemoryStream::create(buff, rc1::pngVRBack_size);
}
vrlive::Stream* getBtnPauseStream()
{
    char* buff = new char[rc1::pngVRPause_size];
    
    memcpy(buff, rc1::pngVRPause, rc1::pngVRPause_size);
    
    return vrlive::MemoryStream::create(buff, rc1::pngVRPause_size);
}
vrlive::Stream* getBtnForwardStream()
{
    char* buff = new char[rc1::pngVRkuaijin_size];
    
    memcpy(buff, rc1::pngVRkuaijin, rc1::pngVRkuaijin_size);
    
    return vrlive::MemoryStream::create(buff, rc1::pngVRkuaijin_size);
}
vrlive::Stream* getBtnBackwardStream()
{
    char* buff = new char[rc1::pngVRkuaitui_size];
    
    memcpy(buff, rc1::pngVRkuaitui, rc1::pngVRkuaitui_size);
    
    return vrlive::MemoryStream::create(buff, rc1::pngVRkuaitui_size);
}
vrlive::Stream* getBackgroundStream()
{
    char* buff = new char[rc1::pngVRBackGround_size];
    
    memcpy(buff, rc1::pngVRBackGround, rc1::pngVRBackGround_size);
    
    return vrlive::MemoryStream::create(buff, rc1::pngVRBackGround_size);
}

namespace vrlive {
    DefaultMenuItem* DefaultMenuItem::create()
    {
        auto defmenu = new DefaultMenuItem();
        
        if (!defmenu->init())
        {
            delete defmenu;
            defmenu = nullptr;
        }
        return defmenu;
    }
    
    void DefaultMenuItem::draw(Camera* camera)
    {
        auto mat = camera->getInverseViewMatrix();
        auto dir = Vector3(mat.m[8], mat.m[9], mat.m[10]);
        if (dir.y <= 0.4f)
        {
            Vector3 z(dir.x, 0.f, dir.z);
            Vector3 y(0.f, 1.f, 0.f);
            Vector3 x;
            Vector3::cross(y, z, &x);
            mat.m[0] = x.x; mat.m[1] = x.y; mat.m[2] = x.z;
            mat.m[4] = y.x; mat.m[5] = y.y; mat.m[6] = y.z;
            mat.m[8] = z.x; mat.m[9] = z.y; mat.m[10] = z.z;
            setTransformMatrix(mat);
        }
        Node::draw(camera);
    }
    
    bool DefaultMenuItem::init()
    {
        Stream* stream = getBtnBackStream();
        _back = MenuItem::create(stream, 1.f, 1.f);
        stream->release();
        
        stream = getBtnForwardStream();
        _forward = MenuItem::create(stream, 1.f, 1.f);
        stream->release();
        stream = getBtnBackwardStream();
        _backward = MenuItem::create(stream, 1.f, 1.f);
        stream->release();
        stream = getBtnPauseStream();
        _pause = MenuItem::create(stream, 1.f, 1.f);
        stream->release();
        stream = getBackgroundStream();
        _background = Billboard::create(stream, 6.f, 2.f);
        stream->release();
        
        _back->setName("back");
        _forward->setName("forward");
        _backward->setName("backward");
        _pause->setName("pause");
        
        _background->setTranslation(Vector3(0, -20, -20));
        _back->setTranslation(Vector3(-1.8, -20, -20));
        _pause->setTranslation(Vector3(-0.6, -20, -20));
        _forward->setTranslation(Vector3(0.6, -20, -20));
        _backward->setTranslation(Vector3(1.8, -20, -20));
        
        addChild(_background);
        addChild(_back);
        addChild(_forward);
        addChild(_backward);
        addChild(_pause);
        
        
        _back->release();
        _forward->release();
        _backward->release();
        _pause->release();
        _background->release();
        
        return true;
    }
    DefaultMenuItem::DefaultMenuItem()
    : _backStream(nullptr)
    , _forwardStream(nullptr)
    , _pauseStream(nullptr)
    , _backwardStream(nullptr)
    , _backgroundStream(nullptr)
    {
        
    }
    DefaultMenuItem::~DefaultMenuItem()
    {
        
    }
}
