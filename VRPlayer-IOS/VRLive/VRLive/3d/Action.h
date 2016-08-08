//
// Created by tony
//
#ifndef ACTION_H_
#define ACTION_H_
#include "Vector3.h"
#include "Matrix.h"
#include <string>
#include "Node.h"
#include "Ref.h"
#include "Vector4.h"

namespace vrlive {
    
    class Action : public Ref
    {
    public:
        
        void setTarget(Node* node) { _target = node; }
        
        Node* getTarget() const { return _target; }
        
        virtual void update(float t);
        
    protected:
        Action();
        virtual ~Action();
        
        Node* _target;
        float _curtime;
    };
    
    class MoveLineAction: public Action
    {
    public:
        
        static MoveLineAction* create(const Vector3& start, const Vector3& end, float t);
        
        virtual void update(float t);
        
    protected:
        MoveLineAction();
        virtual ~MoveLineAction();
        
        Vector3 _start;
        Vector3 _end;
        float _duration;
    };
    
    class RemoveSelfAction: public Action{
        
    };
    
    class RotateAction : public Action
    {
        
    };
}

#endif
