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
    class Texture;
    class Camera;
    
    class Billboard : public Node
    {
    public:
        static Billboard* create(const std::string& filepath, float width, float height);
        
        static Billboard* create(Stream* stream, float width, float height);
        
        virtual void draw(Camera* camera);
        
    protected:
        virtual bool init(Texture* texture, float width, float height);
        Billboard();
        virtual ~Billboard();

    };
}

#endif
