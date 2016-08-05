//
// Created by tony
//
#ifndef SCENE_H_
#define SCENE_H_

#include "Ref.h"
#include "Node.h"
#include "Ray.h"

namespace vrlive {
    
class Camera;
    class CameraCursor;
    class DefaultMenuItem;

class Scene : public Node
{
public:
    static Scene* create();
        
    void draw();
    
    void setCamera(Camera* camera);
    
    Camera* getCamera() const { return _camera; }
    
    Node* cursorCast();
    
    void setWidth(int width) { _width = width; }
    
    int getWidth() const { return _width; }
    
    int getHeight() const { return _height; }
    
    void setHeight(int height) { _height = height; }
    
protected:
    void init();
    
    bool cursorCast(Node* node, const Ray& ray, float* distance, Node* &castnode);
    
    void drawNode(Node* node);
    Scene();
    ~Scene();
    
    Camera* _camera;
    int _width;
    int _height;
    
    CameraCursor* _cursor;
    DefaultMenuItem* _defMenu;
};

}
#endif
