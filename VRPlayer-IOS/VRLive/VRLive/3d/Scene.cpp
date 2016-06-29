#include "Scene.h"
#include "Camera.h"
#include "CameraCursor.h"
#include "Sprite3D.h"
#include "Base.h"

namespace vrlive {
    
    
Scene* Scene::create()
{
    auto scene = new Scene();
        
    scene->init();
        
    return scene;
}

void Scene::setCamera(Camera* camera)
    {
        camera->addRef();
        if (_camera)
        _camera->release();
        
        _camera = camera;
    }
    
void Scene::init()
{
    _cursor = CameraCursor::create("", 1.f, 1.f);
}

void Scene::draw()
{
    GLint texture;
    GLint texBind;
    GLint vertexbuffer;
    GLint idxbuffer;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &texture);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &texBind);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vertexbuffer);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &idxbuffer);
    
    for (auto it : _children) {
        drawNode(it);
    }
    if (_cursor)
    {
        _cursor->draw(_camera);
    }
    
    glActiveTexture(texture);
    glBindTexture(GL_TEXTURE_2D, texBind);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxbuffer);
}
 
    void Scene::drawNode(Node* node)
    {
        auto sprite = node->getSprite3D();
        if (sprite)
        {
            sprite->draw(_camera);
        }
    }
    
Scene::Scene()
    : _camera(nullptr)
    , _cursor(nullptr)
{
}


Scene::~Scene()
{
    if (_camera)
    {
        _camera->release();
        _camera = nullptr;
    }
    if (_cursor)
    {
        _cursor->release();
        _cursor = nullptr;
    }
}
    
}
