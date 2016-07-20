//
// Created by tony
//
#ifndef DEFMENUITEM_H_
#define DEFMENUITEM_H_
#include "Vector3.h"
#include "Matrix.h"
#include "Node.h"
#include "BoundSphere.h"
#include "Ref.h"
#include "BillBoard.h"

namespace vrlive {
    class Scene;
    class MenuItem;
    class Stream;
    class DefaultMenuItem : public Node
    {
    public:
        static DefaultMenuItem* create();
        
        virtual void draw(Camera* camera);
        
    protected:
        bool init();
        DefaultMenuItem();
        virtual ~DefaultMenuItem();

    protected:
        MenuItem* _back;
        MenuItem* _forward;
        MenuItem* _backward;
        MenuItem* _pause;
        Billboard* _background;
        
        Stream* _backStream;
        Stream* _forwardStream;
        Stream* _backwardStream;
        Stream* _pauseStream;
        Stream* _backgroundStream;
    };
}

#endif
