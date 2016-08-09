//
// Created by tony
//
#ifndef ACTION_H_
#define ACTION_H_
#include <map>
#include <vector>
#include <string>
#include "Vector3.h"
#include "Matrix.h"
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
    public:
        static RemoveSelfAction* create(float duration);
        
        virtual void update(float t);
        
    protected:
        RemoveSelfAction();
        virtual ~RemoveSelfAction();
        
        float _duration;
    };
    
    class RotateAction : public Action
    {
        
    };
    
    class ScaleAction: public Action
    {
    public:
        static ScaleAction* create(float scaleFrom, float scaleTo, float duration);
        
        virtual void update(float t);
        
    protected:
        ScaleAction();
        virtual ~ScaleAction();
        
        float _scaleFrom;
        float _scaleTo;
        float _duration;
    };
    
    class ActionMgr
    {
    public:
        static ActionMgr* getInstance();
        
        static void destroyInstance();
        
        void init();
        
        void addAction(Action* action);
        
        void removeActionByNode(Node* node);
        
        void removeAction(Action* action);
        
        void update();
        
    protected:
        ActionMgr();
        ~ActionMgr();
        
        static ActionMgr* _instance;
        std::vector<Action*> _actions;
        double _lasttime;
    };
}

#endif
