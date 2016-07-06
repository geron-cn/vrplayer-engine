//
// Created by tony
//
#ifndef SPRITE3D_H_
#define SPRITE3D_H_

#include <vector>

#include "Ref.h"
#include "BoundSphere.h"
#include "Vector4.h"

namespace vrlive {
    class GLProgram;
    class VertexBuffer;
    class Camera;
    class Node;
    class Texture;
    
    class Sprite3D : public Ref
    {
        friend class Node;
    public:
        static Sprite3D* create(const std::vector<float>& pos, const std::vector<float>& texCoord, const std::vector<unsigned short>& indices);
        
        void setNode(Node* node) { _node = node; }
        
        void setTexture(Texture* texture);
        
        Texture* getTexture() const { return _texture; }
        
        BoundSphere* getBoundSphere() const { return _boundsphere; }
        
        virtual void draw(Camera* camera);
        
        void setColor(const Vector4& color) { _color = color; }
        
    protected:
        bool init(const std::vector<float>& pos, const std::vector<float>& texCoord, const std::vector<unsigned short>& indices);
        
        Sprite3D();
        virtual ~Sprite3D();
        
    protected:
        Vector4   _color;
        GLProgram* _program;
        VertexBuffer* _buffer;
        Node*         _node;
        Texture*      _texture;
        BoundSphere*   _boundsphere;
    };
}

#endif
