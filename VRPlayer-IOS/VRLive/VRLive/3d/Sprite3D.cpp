#include <assert.h>
#include <math.h>
#include "Ref.h"
#include "Sprite3D.h"
#include "GLProgram.h"
#include "VertexBuffer.h"
#include "Camera.h"
#include "Matrix.h"
#include "Node.h"
#include "Texture.h"

namespace vrlive {
    
    Sprite3D* Sprite3D::create(const std::vector<float>& pos, const std::vector<float>& texCoord, const std::vector<unsigned short>& indices)
    {
        auto sprite = new Sprite3D();
        
        if (!sprite->init(pos, texCoord, indices))
        {
            delete sprite;
            sprite = nullptr;
        }
        
        return sprite;
    }
    
    bool Sprite3D::init(const std::vector<float>& pos, const std::vector<float>& texCoord, const std::vector<unsigned short>& indices)
    {
        std::vector<float> normal;
        _buffer = VertexBuffer::create(pos, normal, texCoord, indices);
        if (_buffer == nullptr)
            return false;
        
        size_t count = pos.size() / 3;
        std::vector<Vector3> verts;
        size_t i;
        for (i = 0; i < count; i++)
        {
            size_t idx = i * 3;
            verts.push_back(Vector3(pos[idx], pos[idx+1], pos[idx+2]));
        }
        
        Vector3 center;
        float maxdistance = 0.f;
        for(i = 0; i < count; i++)
        {
            Vector3 pt1 = verts[i];
            for (size_t j = i+1; j < count; j++) {
                Vector3 pt2 = verts[j];
                float dis = (pt1 - pt2).lengthSquared();
                if (dis > maxdistance)
                {
                    maxdistance = dis;
                    center = (pt1 + pt2) * 0.5f;
                }
            }
        }
        _boundsphere = BoundSphere::create(center, sqrtf(maxdistance) * 0.5f);
        
        _program = GLProgramCache::getInstance()->createOrGet(GLProgramCache::PositionTexCoord);//GLProgram::create(s_vs, s_fs);
        return true;
    }
    
    void Sprite3D::setTexture(Texture* texture)
    {
        if (texture)
            texture->addRef();
        if (_texture)
            _texture->release();
        _texture = texture;
    }
    
    void Sprite3D::draw(Camera* camera)
    {
        
        if (_program && _buffer)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            _program->bind();
            
            auto posLocation = _program->getAttribute("a_Position");
            auto texCoordLocation = _program->getAttribute("a_TexCoordinate");
            
            _buffer->bind(posLocation, texCoordLocation);
            if (_texture)
                _texture->bind();
            
            auto mvp = camera->getViewProjectionMatrix();
            if (_node)
            {
//                auto m2 = _node->getWorldTransformMatrix();
//                m2.multiply(mvp);
//                mvp = m2;
                mvp.multiply(_node->getWorldTransformMatrix());
            }
            auto location = _program->getUniform("u_MVPMatrix");
            glUniformMatrix4fv(location, 1, GL_FALSE, mvp.m);
            
            location = _program->getUniform("u_Texture");
            glUniform1i(location, 0);
            
            location = _program->getUniform("u_Color");
            glUniform4f(location, _color.x, _color.y, _color.z, _color.w);
            
            glDrawElements(GL_TRIANGLES, _buffer->getIndexCount(), GL_UNSIGNED_SHORT, 0);
    
            _program->unbind();
            _buffer->unbind();
        }
    }
    
    Sprite3D::Sprite3D()
    : _program(nullptr)
    , _buffer(nullptr)
    , _node(nullptr)
    , _texture(nullptr)
    , _boundsphere(nullptr)
    , _color(1.f, 1.f, 1.f, 1.f)
    {
        
    }
    
    Sprite3D::~Sprite3D()
    {
//        if (_program)
//        {
//            _program->release();
//            _program = nullptr;
//        }
        if (_buffer)
        {
            _buffer->release();
            _buffer = nullptr;
        }
        if (_texture)
        {
            _texture->release();
            _texture = nullptr;
        }
        if (_boundsphere)
        {
            _boundsphere->release();
            _boundsphere = nullptr;
        }
    }
    
}
