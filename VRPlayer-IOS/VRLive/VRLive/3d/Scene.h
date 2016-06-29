//
// Created by tony
//
#ifndef SCENE_H_
#define SCENE_H_

#include "Ref.h"
#include "Node.h"

namespace vrlive {
    
class Camera;
    class CameraCursor;

class Scene : public Node
{
public:
    static Scene* create();
        
    void draw();
    
    void setCamera(Camera* camera);
    
    Camera* getCamera() const { return _camera; }
    
protected:
    void init();
    
    void drawNode(Node* node);
    Scene();
    ~Scene();
    
    Camera* _camera;
    
    CameraCursor* _cursor;
    
};

}
#endif
