//
// Created by tony
//
#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <string>
#include "Base.h"
#include "Ref.h"

namespace vrlive {
    class Stream;
    
    class Texture : public Ref
    {
    public:
        static Texture* create(Stream* stream);
        
        GLuint getHandle() const { return _handle; }
        
        unsigned int getWidth() const { return _width; }
        
        unsigned int getHeight() const { return _height; }
        
        void bind();
        
    protected:
        bool init(Stream* stream);
        Texture();
        Texture(const Texture& copy);
        virtual ~Texture();
        
    private:
        unsigned int _width;
        unsigned int _height;
        GLuint _handle;
    };
}

#endif
