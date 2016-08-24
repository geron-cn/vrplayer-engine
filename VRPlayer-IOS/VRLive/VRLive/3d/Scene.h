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
    
    void setCursorVisible(bool visible);

    void setWidth(int width);
    
    int getWidth() const { return _width; }
    
    int getHeight() const { return _height; }
    
    void setHeight(int height);
    
    DefaultMenuItem* getDefMenuItem() const { return _defMenu; }
    
protected:
    void init();
    
    bool cursorCast(Node* node, const Ray& ray, float* distance, Node* &castnode);
    
    void drawNode(Node* node);

    // recursivelly set children position dirty
    void recursiveSetPosDiry(Node* node);
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
