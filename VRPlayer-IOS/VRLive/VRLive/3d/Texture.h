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
        enum Format
        {
            UNKNOWN = 0,
            RGB,
            RGB888 = RGB,
            RGB565,
            RGBA,
            RGBA8888 = RGBA,
            RGBA4444,
            RGBA5551,
            ALPHA,
            DEPTH,
        };
        
        static Texture* create(Stream* stream);
        
        static Texture* create(Format format, int width, int height, const unsigned char* data);
        
        void setTextureData(const unsigned char* data);
        
        GLuint getHandle() const { return _handle; }
        
        unsigned int getWidth() const { return _width; }
        
        unsigned int getHeight() const { return _height; }
        
        void bind();
        
    protected:
        bool init(Stream* stream);
        Texture();
        Texture(const Texture& copy);
        virtual ~Texture();
        
        static GLint getFormatInternal(Format format);
        static GLenum getFormatTexel(Format format);
        static size_t getFormatBPP(Format format);
        
    private:
        unsigned int _width;
        unsigned int _height;
        GLuint _handle;
        GLint _internalFormat;
        GLenum _texelType;
    };
}

#endif
