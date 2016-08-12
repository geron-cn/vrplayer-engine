#include <jni.h>
#include "StringTextureUtil.h"
#include "../FileUtils/FileUtils.h"
#include "../jnihelper.h"

namespace vrlive
{
float StringTextureUtil::getScaleFactor()
{
    return 1.0f;
}

BitmapDC::BitmapDC()
: _data(nullptr)
, _width(0)
, _height(0)
{
}

BitmapDC::~BitmapDC(void)
{
}

bool BitmapDC::getBitmapFromJavaShadowStroke( const char *text,
                                    int nWidth,
                                    int nHeight,
                                    TextAlign eAlignMask,
                    const FontDefinition& textDefinition )
{
        // Do a full lookup for the font path using FileUtils in case the given font name is a relative path to a font file asset,
        // or the path has been mapped to a different location in the app package:
        std::string fullPathOrFontName = textDefinition._fontName;

LOG("get data from java %s", fullPathOrFontName.c_str());
        /**create bitmap
        * this method call Cococs2dx.createBitmap()(java code) to create the bitmap, the java code
        * will call Java_org_cocos2dx_lib_Cocos2dxBitmap_nativeInitBitmapDC() to init the width, height
        * and data.
        * use this approach to decrease the jni call number
        */

auto className = "com/vrlive/vrlib/common/BitmapHelper";
    auto methodName = "createTextBitmapShadowStroke";
    auto paramCode = "([BLjava/lang/String;IIIIIIIIZFFFFZIIIIFZI)Z";
JniMethodInfo methodInfo;
           if (!getStaticMethodInfo(methodInfo, className, methodName, paramCode))
           {
               LOG("error in method");
               return false;
           }
        int count = strlen(text);
        jbyteArray strArray = methodInfo.env->NewByteArray(count);
        methodInfo.env->SetByteArrayRegion(strArray, 0, count, reinterpret_cast<const jbyte*>(text));
        jstring jstrFont = methodInfo.env->NewStringUTF(fullPathOrFontName.c_str());
           // Do 
        if(!methodInfo.env->CallStaticBooleanMethod(methodInfo.classID,
            methodInfo.methodID, strArray,
            jstrFont, textDefinition._fontSize,
            (int) textDefinition._fontFillColor.x * 255,
            (int) textDefinition._fontFillColor.y * 255, 
            (int) textDefinition._fontFillColor.z * 255,
             textDefinition._fontAlpha,
            eAlignMask, nWidth, nHeight, 
        //    textDefinition._shadow._shadowEnabled, textDefinition._shadow._shadowOffset.width, -textDefinition._shadow._shadowOffset.height, 
        //    textDefinition._shadow._shadowBlur, textDefinition._shadow._shadowOpacity, 
        //    textDefinition._stroke._strokeEnabled, textDefinition._stroke._strokeColor.x, textDefinition._stroke._strokeColor.y, 
        //                                            textDefinition._stroke._strokeColor.z, textDefinition._stroke._strokeAlpha, textDefinition._stroke._strokeSize,
            0/*false*/, 1, 1, 1, 0, 1,
            0/*false*/, 1, 1, 1, 1, 12,
                textDefinition._enableWrap, textDefinition._overflow))
        {
            LOG("get data from java failed");
            return false;
        }
        methodInfo.env->DeleteLocalRef(strArray);
        methodInfo.env->DeleteLocalRef(jstrFont);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);

        return true;
}

JniMethodInfo StringTextureUtil::getBitmapFunc;
BitmapDC      StringTextureUtil::bitmapDC;

Data StringTextureUtil::getTextureDataForText(const char * text, const FontDefinition& textDefinition, TextAlign align, int &width, int &height, bool& hasPremultipliedAlpha)
{
    Data ret;
    do 
    {
        if(! bitmapDC.getBitmapFromJavaShadowStroke(text, 
            (int)textDefinition._dimensions[0], 
            (int)textDefinition._dimensions[1], 
            align, textDefinition )) { break;};

        width = bitmapDC._width;
        height = bitmapDC._height;
        ret.fastSet(bitmapDC._data,width * height * 4);
        hasPremultipliedAlpha = true;
    } while (0);

    if(!ret.isNull())
    {
        LOG("%s", ret.getBase64().c_str());
    }
    else
    {
        LOG("get texture from text failed! ");
    }
    return ret;
}

  
}