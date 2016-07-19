#include "Texture.h"
#include "Image.h"
#include "Stream.h"

namespace vrlive {
    
    Texture* Texture::create(Stream* stream)
    {
        Texture* tex = new Texture();
        
        tex->init(stream);
        
        return tex;
    }
    
    bool Texture::init(Stream* stream)
    {
        Image* image = Image::create(stream);
        
        // Create the texture.
        GLuint textureId;
        ( glGenTextures(1, &textureId) );
        ( glBindTexture(GL_TEXTURE_2D, textureId) );
        ( glPixelStorei(GL_UNPACK_ALIGNMENT, 1) );
        
        _width = image->getWidth();
        _height = image->getHeight();
        unsigned char* data = image->getData();
        
        GLenum format = (image->getFormat() == Image::RGB ? GL_RGB : GL_RGBA);
        // Load the texture
        {
            // Texture 2D
            ( glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)format, _width, _height, 0, (GLenum)format, GL_UNSIGNED_BYTE, data) );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        
        _handle = textureId;
        image->release();
        
        glBindTexture(GL_TEXTURE_2D, 0);
        
        return true;
    }
    
    GLint Texture::getFormatInternal(Format format)
    {
        switch (format)
        {
            case Texture::RGB888:
            case Texture::RGB565:
                return GL_RGB;
            case Texture::RGBA8888:
            case Texture::RGBA4444:
            case Texture::RGBA5551:
                return GL_RGBA;
            case Texture::ALPHA:
                return GL_ALPHA;
            case Texture::DEPTH:
#if !defined(OPENGL_ES) || defined(GL_ES_VERSION_3_0)
                return GL_DEPTH_COMPONENT32F;
#else
                return GL_DEPTH_COMPONENT;
#endif
            default:
                return 0;
        }
    }
    
    GLenum Texture::getFormatTexel(Format format)
    {
        switch (format)
        {
            case Texture::RGB888:
            case Texture::RGBA8888:
            case Texture::ALPHA:
                return GL_UNSIGNED_BYTE;
            case Texture::RGB565:
                return GL_UNSIGNED_SHORT_5_6_5;
            case Texture::RGBA4444:
                return GL_UNSIGNED_SHORT_4_4_4_4;
            case Texture::RGBA5551:
                return GL_UNSIGNED_SHORT_5_5_5_1;
            case Texture::DEPTH:
#if !defined(OPENGL_ES) || defined(GL_ES_VERSION_3_0)
                return GL_FLOAT;
#else
                return GL_UNSIGNED_INT;
#endif
            default:
                return 0;
        }
    }
    
    size_t Texture::getFormatBPP(Format format)
    {
        switch (format)
        {
            case Texture::RGB565:
            case Texture::RGBA4444:
            case Texture::RGBA5551:
                return 2;
            case Texture::RGB888:
                return 3;
            case Texture::RGBA8888:
                return 4;
            case Texture::ALPHA:
                return 1;
            default:
                return 0;
        }
    }
    
    Texture* Texture::create(Format format, int width, int height, const unsigned char* data)
    {
        int oldtextureId;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &oldtextureId);
        
        GLenum target = GL_TEXTURE_2D;
        
        GLint internalFormat = getFormatInternal(format);
        GLenum texelType = getFormatTexel(format);
        // Create the texture.
        GLuint textureId;
        ( glGenTextures(1, &textureId) );
        ( glBindTexture(target, textureId) );
        ( glPixelStorei(GL_UNPACK_ALIGNMENT, 1) );
        size_t bpp = getFormatBPP(format);
        
        GLenum f = (format == Texture::DEPTH) ? GL_DEPTH_COMPONENT : internalFormat;
        ( glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, f, texelType, data) );
        
        if (format == Texture::DEPTH)
        {
            ( glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST) );
            ( glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST) );
            ( glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
            ( glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
#if !defined(OPENGL_ES) || defined(GL_ES_VERSION_3_0) && GL_ES_VERSION_3_0
            ( glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, GL_NONE) );
#endif
        }
        else
        {
            ( glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
            ( glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        Texture* texture = new Texture();
        texture->_handle = textureId;
        texture->_width = width;
        texture->_height = height;
        texture->_internalFormat = internalFormat;
        texture->_texelType = texelType;
        
        glBindTexture(GL_TEXTURE_2D, oldtextureId);
        return texture;
    }
    
    void Texture::setTextureData(const unsigned char* data)
    {
        int oldtextureId;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &oldtextureId);
        
        glBindTexture(GL_TEXTURE_2D, _handle);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, _internalFormat, _texelType, data);
        
        glBindTexture(GL_TEXTURE_2D, oldtextureId);
    }
    
    void Texture::bind()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _handle);
    }
    
    Texture::Texture()
    {
        
    }
    Texture::~Texture()
    {
        
    }
}
