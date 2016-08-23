#include <math.h>
#include "Ray.h"
#include "DefaultMenuItem.h"
#include "Texture.h"
#include "Sprite3D.h"
#include "Camera.h"
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
    
    void DefaultMenuItem::update(Scene* scene)
    {
        auto camera = scene->getCamera();
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
        _isVisible = (dir.y > 0.4f);
        
        Node::update(scene);
    }
        
    bool DefaultMenuItem::init()
    {
    #define DELETE_PNG_BUFFER {stream->close();stream->release();}
        Stream* stream = getBtnBackStream();
        _back = MenuItem::create(stream, 2.f, 2.f);
        DELETE_PNG_BUFFER;
        
        stream = getBtnForwardStream();
        _forward = MenuItem::create(stream, 2.f, 2.f);
        DELETE_PNG_BUFFER;

        stream = getBtnBackwardStream();
        _backward = MenuItem::create(stream, 2.f, 2.f);
        DELETE_PNG_BUFFER;

        stream = getBtnPauseStream();
        _pause = MenuItem::create(stream, 2.f, 2.f);
        DELETE_PNG_BUFFER;

        stream = getBackgroundStream();
         _background = Billboard::create(stream, 10.f, 3.f);
        DELETE_PNG_BUFFER;
    #undef DELETE_PNG_BUFFER

        _back->setName("back");
        _forward->setName("forward");
        _backward->setName("backward");
        _pause->setName("pause");
        
         _background->setTranslation(Vector3(0, -20, -20));
        _back->setTranslation(Vector3(-3.f, -20, -20));
        _pause->setTranslation(Vector3(-1.f, -20, -20));
        _forward->setTranslation(Vector3(1.f, -20, -20));
        _backward->setTranslation(Vector3(3.f, -20, -20));
        
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
    
    void DefaultMenuItem::showPlayerMenu(bool isVisible)
    {
        if (_background)
            _background->setVisible(isVisible);
        if (_back)
            _back->setVisible(isVisible);
        if (_forward)
            _forward->setVisible(isVisible);
        if (_backward)
            _backward->setVisible(isVisible);
        if (_pause)
            _pause->setVisible(isVisible);
    }
    
    void DefaultMenuItem::setCustomMenuShow(bool visible)
    {
        for(auto child : _children)
        {
            if(child != _background
            && child != _back
            && child != _forward
            && child != _backward
            && child != _pause)
            {
                child->setVisible(visible);
            }
        }
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
