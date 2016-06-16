//
// Created by tony
//
#ifndef CAMERACURSOR_H_
#define CAMERACURSOR_H_
#include "Vector3.h"
#include "Ref.h"
#include <string>

namespace vrlive {
    class GLProgram;
    class CameraCursor : public Ref
    {
    public:
        CameraCursor* create(const std::string& imagePath, float width, float height);
        
        void setSize(float width, float height);
        
        void draw();
        
    protected:
        
        bool init(float width, float height);
        
        CameraCursor();
        
        ~CameraCursor();
        
    
        GLProgram* _program;
        float      _vertex[];
    };
}

#endif
