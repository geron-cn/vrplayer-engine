//
// Created by tony
//
#ifndef BILLBOARD_H_
#define BILLBOARD_H_
#include "Vector3.h"
#include "Matrix.h"
#include "Node.h"
#include "BoundSphere.h"

namespace vrlive {
    class Stream;
    class Camera;
    class Billboard : public Node
    {
    public:
        static Billboard* create(Stream* stream, float width, float height);
        
        virtual void draw(Camera* camera);
        
    protected:
        bool init(Stream* stream, float width, float height);
        
        Billboard();
        virtual ~Billboard();

    };
}

#endif
