//
// Created by tony
//
#ifndef IMAGE_H_
#define IMAGE_H_
#include "Ref.h"

namespace vrlive {
    class Stream;
    /**
     * Defines an image buffer of RGB or RGBA color data.
     *
     * Currently only supports loading from .png image files.
     */
    class Image : public Ref
    {
    public:
        
        /**
         * Defines the set of supported color formats.
         */
        enum Format
        {
            RGB,
            RGBA
        };
        
        
        static Image* create(Stream* stream);
        
        /**
         * Creates an image from the data provided
         *
         * @param width The width of the image data.
         * @param height The height of the image data.
         * @param format The format of the image data.
         * @param data The image data. If NULL, the data will be allocated.
         * @return The newly created image.
         * @script{create}
         */
        static Image* create(unsigned int width, unsigned int height, Format format, unsigned char* data);
        
        /**
         * Gets the image's raw pixel data.
         *
         * @return The image's pixel data.
         * @script{ignore}
         */
        unsigned char* getData() const
        {
            return _data;
        }
        
        /**
         * Gets the image's format.
         *
         * @return The image's format.
         */
        Format getFormat() const
        {
            return _format;
        }
        
        /**
         * Gets the height of the image.
         *
         * @return The height of the image.
         */
        unsigned int getHeight() const
        {
            return _height;
        }
        
        /**
         * Gets the width of the image.
         *
         * @return The width of the image.
         */
        unsigned int getWidth() const
        {
            return _width;
        }
        
    private:
        
        /**
         * Constructor.
         */
        Image();
        
        /**
         * Destructor.
         */
        ~Image();
        
        /**
         * Hidden copy assignment operator.
         */
        Image& operator=(const Image&);
        
        unsigned char* _data;
        Format _format;
        unsigned int _width;
        unsigned int _height;
    };
}

#endif
