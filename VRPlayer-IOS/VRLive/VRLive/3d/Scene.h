//
// Created by tony
//
#ifndef SCENE_H_
#define SCENE_H_

#include "Ref.h"

namespace vrlive {
    
class Camera;

class Scene : Ref
{
public:
    static Scene* getInstance();
    
    static void destroyInstance();
    
    void draw();
    
    
protected:
    Scene();
    ~Scene();
    
    Camera* _camera;
    
    static Scene* _instance;
};

}
#endif
