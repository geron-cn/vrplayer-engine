//
// Created by tony
//
#ifndef CAMERACURSOR_H_
#define CAMERACURSOR_H_
#include "Vector3.h"
#include "Ref.h"
#include "Node.h"
#include <string>

namespace vrlive {
    class GLProgram;
    class Camera;
    class Sprite3D;
    
    class CameraCursor : public Node
    {
    public:
        static CameraCursor* create(const std::string& imagePath, float width, float height);
        
        void setScale(float scale);
        
        
        void draw(Camera* camera);
        
    protected:
        
        bool init(float width, float height);
        
        CameraCursor();
        
        ~CameraCursor();
        
    };
}

#endif
