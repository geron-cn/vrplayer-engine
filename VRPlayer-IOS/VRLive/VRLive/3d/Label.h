//
// Created by tony
//
#ifndef LABEL_H_
#define LABEL_H_
#include "Vector3.h"
#include "Matrix.h"
#include <string>
#include "Node.h"
#include "StringTextureUtil.h"
#include "BoundSphere.h"
#include "Vector4.h"

namespace vrlive {
    class Stream;
    class Camera;
    class Texture;
    class GLProgram;
    class VertexBuffer;
    class Sprite3D;
    class Scene;
    class Label : public Node
    {
    public:
        static Label* create(const std::string& text, const std::string& font, float fontSize, const Vector4& color, int width = 0, int height = 0, TextHAlignment hAlignment = TextHAlignment::LEFT,
                             TextVAlignment vAlignment = TextVAlignment::TOP);
        
        virtual void draw(Camera* camera);
        
        virtual void update(Scene* scene);
        
        void setNormalizedTranslation(float x, float y);
        
        int getWidth() const;
        
        int getHeight() const;
        
    protected:
        
        Label();
        virtual ~Label();

        Texture* _tex;
        Sprite3D* _sprite;
        
        float _normalizedX;
        float _normalizedY;
    };
}

#endif
