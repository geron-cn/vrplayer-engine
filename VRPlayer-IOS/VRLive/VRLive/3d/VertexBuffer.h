//
// Created by tony
//
#ifndef VERTEXBUFFER_H_
#define VERTEXBUFFER_H_

#include <vector>
#include "Ref.h"
#include "Base.h"

namespace vrlive {
    

class VertexBuffer : public Ref
{
public:
    enum class VertexUsage
    {
        POSITION,
        NORMAL,
        TEXCOORD0,
    };
    static VertexBuffer* create(const std::vector<float>& position, const std::vector<float>& normal, const std::vector<float>& texCoord, const std::vector<unsigned short>& indices);
    
    bool hasAttribute(VertexUsage usage) const;
    
    int getIndexCount() const { return (int)_indices.size(); }
    
    void bind(GLuint positionLocation, GLuint texCoordLocation);
    
    void unbind();
    
    
protected:
    
    void initBuffer();
    
    VertexBuffer();
    virtual ~VertexBuffer();
    
    std::vector<float> _vertexes;
    std::vector<VertexUsage> _usages;
    std::vector<unsigned short> _indices;
    GLuint _vertexbuffer;
    GLuint _indexbuffer;
    GLsizei _stride;
};

}
#endif
