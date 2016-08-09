#include "Scene.h"
#include "Camera.h"
#include "CameraCursor.h"
#include "Sprite3D.h"
#include "Base.h"
#include "DefaultMenuItem.h"
#include "GLProgram.h"
#include "Label.h"
#include "Action.h"

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
    ActionMgr::getInstance()->init();
}

void Scene::draw()
{
    ActionMgr::getInstance()->update();
    update(this);
//    auto mat = _camera->getInverseViewMatrix();
//    if(mat.m[9] < 0.4f)
//    {
////        for (auto it : _children) {
////            it->draw(_camera);
////        } // translate menu
//        return;
//    }

    GLint texture;
    GLint texBind;
    GLint vertexbuffer;
    GLint idxbuffer;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &texture);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &texBind);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vertexbuffer);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &idxbuffer);

    glDisable(GL_DEPTH_TEST);
    
    if (_defMenu == nullptr)
    {
        _defMenu = DefaultMenuItem::create();
        addChild(_defMenu);
        _defMenu->release();
    }
    
    if (_cursor && _cursor->isVisible())
    {
        auto lookingNode = cursorCast();
        _camera->setLookingNode(lookingNode);
        _cursor->updatePickUp(lookingNode);
    }
    else
        _cursor->updatePickUp(nullptr);
    
    for (auto it : _children) {
        
        it->draw(_camera);
    }
    if (_cursor && _cursor->isVisible())
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
    
    Node* Scene::cursorCast()
    {
        if (_camera)
        {
            auto mat = _camera->getInverseViewMatrix();
            Vector3 pos(mat.m[12], mat.m[13], mat.m[14]);
            Vector3 dir(-mat.m[8], -mat.m[9], -mat.m[10]);
            Ray ray(pos, dir);
            
            float distance;
            Node* castnode = nullptr;
            cursorCast(this, ray, &distance, castnode);
            return castnode;
        }
        
        
        return nullptr;
    }
    
    bool Scene::cursorCast(Node* node, const Ray& ray, float* distance, Node* &castnode)
    {
        auto sprite = node->getSprite3D();
        if (sprite && !node->getName().empty())
        {
            auto sphere = sprite->getBoundSphere();
            auto mat = node->getWorldTransformMatrix();
            float dis;
            bool intersected = ray.intersect(sphere, &mat, &dis);
            
            if (intersected)
            {
                if (castnode == nullptr)
                {
                    castnode = node;
                    if (*distance)
                        *distance = dis;
                }
                else
                {
                    if (*distance > dis)
                    {
                        castnode = node;
                        *distance = dis;
                    }
                    
                }
            }
        }
        
        for (auto child : node->_children) {
            cursorCast(child, ray, distance, castnode);
        }
        if (castnode)
            return true;
        return false;
    }
    
Scene::Scene()
    : _camera(nullptr)
    , _cursor(nullptr)
    , _defMenu(nullptr)
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
    if (_defMenu)
    {
        _defMenu = nullptr;
    }
    GLProgramCache::destroyInstance();
    ActionMgr::destroyInstance();
}
    
}
