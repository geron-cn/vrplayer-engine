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
