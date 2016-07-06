//
// Created by tony
//
#ifndef MENUITEM_H_
#define MENUITEM_H_
#include "Vector3.h"
#include "Matrix.h"
#include "Node.h"
#include "BoundSphere.h"
#include "BillBoard.h"

namespace vrlive {
    
    class MenuItem : public Billboard
    {
    public:
        static MenuItem* create(Stream* stream, float width, float height);
        
        virtual void draw(Camera* camera);
        
    protected:
        
        
        MenuItem();
        virtual ~MenuItem();

    };
}

#endif
