#include <math.h>
#include "Ray.h"
#include "BillBoard.h"
#include "Texture.h"
#include "Sprite3D.h"
#include "Camera.h"

namespace vrlive {
    Billboard* Billboard::create(Stream* stream, float width, float height)
    {
        auto board = new Billboard();
        auto tex = Texture::create(stream);
        if (board->init(tex, width, height) == false)
        {
            delete board;
            board = nullptr;
        }
        
        return board;
    }
    
    Billboard* Billboard::create(const std::string &filepath, float width, float height)
    {
        auto board = new Billboard();
        auto texture = Texture::create(filepath);
        if (board->init(texture, width, height) == false)
        {
            delete board;
            board = nullptr;
        }
        
        return board;
    }
    
    
    bool Billboard::init(Texture* texture, float width, float height)
    {
        auto tex = texture;
        if (tex == nullptr)
            return false;
        
        //left top
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
        texCoord[1]=1;
        texCoord[2]=0;
        texCoord[3]=0;
        texCoord[4]=1;
        texCoord[5]=1;
        texCoord[6]=1;
        texCoord[7]=0;
        
        std::vector<unsigned short> idx;
        idx.resize(6);
        idx[0]=0;
        idx[1]=1;
        idx[2]=2;
        idx[3]=2;
        idx[4]=1;
        idx[5]=3;
        
        _sprite = Sprite3D::create(pos, texCoord, idx);
        _sprite->setTexture(tex);
        _sprite->setNode(this);
        tex->release();
        
        
        return true;
    }
    void Billboard::draw(Camera* camera)
    {
//        auto mat = camera->getInverseViewMatrix();
//        auto mat2 = getTransformMatrix();
//        
//        memcpy(mat2.m, mat.m, sizeof(float) * 12);
//        
//        setTransformMatrix(mat2);
        
        Node::draw(camera);
    }
    Billboard::Billboard()
    {
        
    }
    Billboard::~Billboard()
    {
        
    }
}
