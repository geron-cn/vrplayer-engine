#include "Scene.h"
#include "Camera.h"

namespace vrlive {
    

Scene* Scene::_instance = nullptr;

Scene* Scene::getInstance()
{
    if (_instance != nullptr)
    {
        _instance = new Scene();
    }
    return _instance;
}
    
void Scene::destroyInstance()
    {
        if (_instance)
        {
            delete _instance;
            _instance = nullptr;
        }
    }

void Scene::draw()
{
    
}
    
    
Scene::Scene()
    : _camera(nullptr)
{
}


Scene::~Scene()
{
    if (_camera)
    {
        _camera->release();
        _camera = nullptr;
    }
}
    
}
