//
// Created by tony
//
#ifndef STRING_TEXTURE_UTIL_H_
#define STRING_TEXTURE_UTIL_H_
#include <string>
#include <stdlib.h>
#include "Base.h"
#include "Ref.h"
#include "Vector3.h"
#include "../FileUtils/Data.h"
#ifdef __ANDROID__
#include <jni.h>
#endif

namespace vrlive {
    
    enum class TextVAlignment
    {
        TOP,
        CENTER,
        BOTTOM
    };
    
    /** @enum TextHAlignment
     * Horizontal text alignment type.
     *
     * @note If any of these enums are edited and/or reordered, update Texture2D.m.
     */
    enum class TextHAlignment
    {
        LEFT,
        CENTER,
        RIGHT
    };
    
    /** Defines the alignment of text. */
    enum class TextAlign
    {
        CENTER        = 0x33, /** Horizontal center and vertical center. */
        TOP           = 0x13, /** Horizontal center and vertical top. */
        TOP_RIGHT     = 0x12, /** Horizontal right and vertical top. */
        RIGHT         = 0x32, /** Horizontal right and vertical center. */
        BOTTOM_RIGHT  = 0x22, /** Horizontal right and vertical bottom. */
        BOTTOM        = 0x23, /** Horizontal center and vertical bottom. */
        BOTTOM_LEFT   = 0x21, /** Horizontal left and vertical bottom. */
        LEFT          = 0x31, /** Horizontal left and vertical center. */
        TOP_LEFT      = 0x11, /** Horizontal left and vertical top. */
    };
    
    struct FontDefinition
    {
    public:
        
        FontDefinition()
        : _fontSize(0)
        , _fontAlpha(255)
        , _enableWrap(true)
        , _fontFillColor(1.0f, 1.0f, 1.0f)
        , _overflow(0)
        {}
        
        /// font name
        std::string           _fontName;
        /// font size
        int                   _fontSize;
        
        /// horizontal alignment
        TextHAlignment        _alignment;
        /// vertical alignment
        TextVAlignment _vertAlignment;

        float                  _dimensions[2];
        /// font color
        Vector3               _fontFillColor;
        /// font alpha
        GLubyte               _fontAlpha;
        /// enable text wrap
        bool                  _enableWrap;
        /** There are 4 overflows: none, clamp, shrink and resize_height.
         *  The corresponding integer values are 0, 1, 2, 3 respectively
         * For more information, please refer to Label::Overflow enum class.
         */
        int                  _overflow;
    };

#ifdef  __ANDROID__
    struct JniMethodInfo
{
    JNIEnv *    env;
    jclass      classID;
    jmethodID   methodID;
};

class BitmapDC
{
public:

    BitmapDC();

    ~BitmapDC(void);

    bool getBitmapFromJavaShadowStroke( const char *text,
                                        int nWidth,
                                        int nHeight,
                                        TextAlign eAlignMask,
                      const FontDefinition& textDefinition );

public:
    int _width;
    int _height;
    unsigned char *_data;
};
    class StringTextureUtil
    {
    public:
         static BitmapDC      bitmapDC;
         static JniMethodInfo getBitmapFunc;
#else
    class StringTextureUtil
        {
        public:
#endif //  __ANDROID__

        // no use in android
        static float getScaleFactor();    
        
        static Data getTextureDataForText(const char * text, const FontDefinition& textDefinition, TextAlign align, int &width, int &height, bool& hasPremultipliedAlpha);  
    };
}

#endif
