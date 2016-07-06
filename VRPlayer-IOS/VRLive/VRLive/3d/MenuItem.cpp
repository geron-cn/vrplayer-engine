#include <math.h>
#include <stdlib.h>
#include "Ray.h"
#include "MenuItem.h"
#include "Texture.h"
#include "Sprite3D.h"
#include "camera.h"

namespace vrlive {
    MenuItem* MenuItem::create(Stream* stream, float width, float height)
    {
        auto board = new MenuItem();
        
        if (board->init(stream, width, height) == false)
        {
            delete board;
            board = nullptr;
        }
        
        return board;
    }
    
    void MenuItem::draw(Camera* camera)
    {
        //looking at me, change my color
        if (_sprite)
        {
            Vector4 color(1.f, 1.f, 1.f, 1.f);
            if (camera->getLookingNode() == this)
            {
                color.set(1.f, 1.f, 0.f, 1.f);
                setScale(Vector3(1.2f, 1.2f, 1.2f));
                
            }
            else{
                setScale(Vector3(1.f, 1.f, 1.f));
            }
            
            _sprite->setColor(color);
        }
        
        Billboard::draw(camera);
    }
    MenuItem::MenuItem()
    {
        
    }
    MenuItem::~MenuItem()
    {
        
    }
}
