#include <math.h>
#include "Ray.h"
#include "Label.h"
#include "Texture.h"
#include "Sprite3D.h"
#include "Camera.h"
#include "GLProgram.h"
#include "VertexBuffer.h"
#include "Scene.h"

// for test
#include "../FileUtils/FileUtils.h"

namespace vrlive {
    
    static Camera* _camera2D = nullptr;
    float Label::s_2dCameraRotAngle = 0;
    
    Label::Label()
    {
        
    }
    Label::~Label()
    {
//        if (_sprite)
//        {
//            _sprite->release();
//            _sprite = nullptr;
//        }
    }
    
    int Label::getWidth() const
    {
        auto tex = getTexture();
        if (tex)
        {
            return tex->getWidth();
        }
        return 0;
    }
    
    int Label::getHeight() const
    {
        auto tex = getTexture();
        if (tex)
        {
            return tex->getHeight();
        }
        return 0;
    }
    
    Label* Label::createWithTexture(const std::string& path)
    {
        auto tex = Texture::create(path);
        auto ret = new Label();
        ret->_id = path;
        ret->initWithTexture(tex);
        tex->release();
        
        return ret;
    }
    
    Label* Label::createWithTexture(Texture* tex)
    {
        auto ret = new Label();
        ret->initWithTexture(tex);
        return ret;
    }

    Texture* Label::getTexture() const
    {
        if(_sprite)
        {
            return _sprite->getTexture();
        }
        return nullptr;
    }
    void Label::initWithTexture(Texture* tex)
    {
        //left top
        int width = tex->getWidth();
        int height = tex->getHeight();
        Vector3 lt(-width*0.5f, height*0.5f, 0.f);
        Vector3 lb(-width*0.5f, -height*0.5f, 0.f);
        Vector3 rt(width*0.5f, height*0.5f, 0.f);
        Vector3 rb(width*0.5f, -height*0.5f, 0.f);
        std::vector<float> pos;
        pos.resize(12);
        pos[0] = lt.x;
        pos[1] = lt.y;
        pos[2] = lt.z;
        pos[3] = lb.x;
        pos[4] = lb.y;
        pos[5] = lb.z;
        pos[6] = rt.x;
        pos[7] = rt.y;
        pos[8] = rt.z;
        pos[9] = rb.x;
        pos[10] = rb.y;
        pos[11] = rb.z;
        
        std::vector<float> texCoord;
        texCoord.resize(8);
        texCoord[0]=0;
        texCoord[1]=0;
        texCoord[2]=0;
        texCoord[3]=1;
        texCoord[4]=1;
        texCoord[5]=0;
        texCoord[6]=1;
        texCoord[7]=1;
        
        std::vector<unsigned short> idx;
        idx.resize(6);
        idx[0]=0;
        idx[1]=1;
        idx[2]=2;
        idx[3]=2;
        idx[4]=1;
        idx[5]=3;
        
        std::vector<float> normal;
        _sprite = Sprite3D::create(pos, texCoord, idx);
        _sprite->setTexture(tex);
        _sprite->setNode(this);
    }
    
    Label* Label::create(const std::string& text, const std::string& font, float fontSize, const Vector4& color, int width, int height, TextHAlignment hAlignment, TextVAlignment vAlignment)
    {
        Label* ret = new Label();
        
        FontDefinition systemFontDef;
        systemFontDef._fontName = font;
        systemFontDef._fontSize = fontSize;
        systemFontDef._alignment = hAlignment;
        systemFontDef._vertAlignment = vAlignment;
        systemFontDef._dimensions[0] = width;
        systemFontDef._dimensions[1] = height;
        systemFontDef._fontFillColor.x = color.x;
        systemFontDef._fontFillColor.y = color.y;
        systemFontDef._fontFillColor.z = color.z;
        systemFontDef._fontAlpha = color.w * 255;
        systemFontDef._enableWrap = true;
        systemFontDef._overflow = 0;
        
        auto tex = Texture::createWithString(text, systemFontDef);
        
        ret->initWithTexture(tex);
        
        return ret;
    }

    void Label::update(Scene* scene)
    {
        int screenWidth = scene->getWidth();
        int screenHeight = scene->getHeight();
        if (_camera2D == nullptr)
        {
            _camera2D = Camera::createOrthographic(screenWidth, screenHeight, (float)screenWidth / screenHeight, -500, 500);
        }
        else if (_camera2D->getOrthWidth() != screenWidth || _camera2D->getOrthHeight() != screenHeight)
        {
            _camera2D->release();
            _camera2D = Camera::createOrthographic(screenWidth, screenHeight, (float)screenWidth / screenHeight, 0.1f, 500);
        }
        if (_camera2D)
        {
            Matrix mat;
            Matrix::createRotation(Vector3(0.f, 0.f, 1.f), s_2dCameraRotAngle * 3.1415926f / 180.f, &mat);
            mat.invert();
            _camera2D->setViewMatrix(mat.m);
        }
//        if (_normalizedX != -1 && _normalizedY != -1)
//        {
//            Vector3 pos(_normalizedX*screenWidth, _normalizedY*screenHeight, 0);
//            setTranslation(pos);
//        }
    }
    
    void Label::rotationZ(float angleZ)
    {
        s_2dCameraRotAngle = -angleZ;
    }
    
    void Label::draw(Camera* camera)
    {
        if (_sprite && _camera2D)
        {
            _sprite->draw(_camera2D);
        }
        for (auto it : _children) {
            it->draw(camera);
        }
    }
}
