//
// Created by tony
//
#ifndef CAMERACURSOR_H_
#define CAMERACURSOR_H_
#include "Vector3.h"
#include "Ref.h"
#include "Node.h"
#include <string>

namespace vrlive {
    class GLProgram;
    class Camera;
    class Sprite3D;
    
    class CameraCursor : public Node
    {
        friend class Scene;
        enum class State
        {
            PickingUp,
            CoolingDown,
        };
    public:
        static CameraCursor* create(const std::string& imagePath, float width, float height);
                
        void setVisible(bool visible) { _show = visible; }
        
        bool isVisible() const { return _show; }
        
        void updatePickUp(Node* pickobj);
        
    protected:
        virtual void draw(Camera* camera);
        bool init(float width, float height);
        
        CameraCursor();
        
        ~CameraCursor();
        
        bool _show;
        int _pickUpCount;
        Node* _pickingObj;
        State _state;
    };
}

#endif
