#include <assert.h>
#include "VertexBuffer.h"
#include "Camera.h"

namespace vrlive {
    
    VertexBuffer* VertexBuffer::create(const std::vector<float>& position, const std::vector<float>& normal, const std::vector<float>& texCoord, const std::vector<unsigned short>& indices)
    {
        assert(position.size());
        auto vertexbuffer = new VertexBuffer();
        
        auto count = position.size() / 3;
        
        bool hasNormal = (normal.size() > 0);
        bool hasTexCoord = (texCoord.size() > 0);
        
        vertexbuffer->_stride = 3;
        vertexbuffer->_usages.push_back(VertexUsage::POSITION);
        if (hasNormal)
        {
            vertexbuffer->_usages.push_back(VertexUsage::NORMAL);
            vertexbuffer->_stride += 3;
        }
        if (hasTexCoord)
        {
            vertexbuffer->_usages.push_back(VertexUsage::TEXCOORD0);
            vertexbuffer->_stride += 2;
        }
        
        vertexbuffer->_vertexes.resize(vertexbuffer->_stride * count);
        for (size_t i = 0; i < count; i++)
        {
            size_t idx = i * vertexbuffer->_stride;
            vertexbuffer->_vertexes[idx++] = position[i*3];
            vertexbuffer->_vertexes[idx++] = position[i*3+1];
            vertexbuffer->_vertexes[idx++] = position[i*3+2];
            if (hasNormal)
            {
                vertexbuffer->_vertexes[idx++] = normal[i*3];
                vertexbuffer->_vertexes[idx++] = normal[i*3+1];
                vertexbuffer->_vertexes[idx++] = normal[i*3+2];
            }
            if (hasTexCoord)
            {
                vertexbuffer->_vertexes[idx++] = texCoord[i*2];
                vertexbuffer->_vertexes[idx++] = texCoord[i*2+1];
            }
        }
        vertexbuffer->_indices = indices;
        
        vertexbuffer->initBuffer();
        
        return vertexbuffer;
    }
    
    void VertexBuffer::initBuffer()
    {
        glGenBuffers(1, &_vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*_vertexes.size(), &_vertexes[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1, &_indexbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * _indices.size(), &_indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    bool VertexBuffer::hasAttribute(VertexUsage usage) const
    {
        for (auto it : _usages) {
            if (it == usage)
                return true;
        }
        return false;
    }
    
    void VertexBuffer::bind(GLuint positionLocation, GLuint texCoordLocation)
    {
        int stride = _stride * sizeof(float);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexbuffer);
        glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, stride, 0);
        glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(positionLocation);
        glEnableVertexAttribArray(texCoordLocation);
    }
    
    void VertexBuffer::unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    VertexBuffer::VertexBuffer()
    : _vertexbuffer(0)
    , _indexbuffer(0)
    {
        
    }
    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &_vertexbuffer);
        glDeleteBuffers(1, &_indexbuffer);
    }
}
