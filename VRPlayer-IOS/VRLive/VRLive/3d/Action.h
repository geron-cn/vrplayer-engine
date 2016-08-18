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
#include "Texture.h"
#include <functional>

namespace vrlive {
    
    class Action : public Ref
    {
    public:
        
        virtual void setTarget(Node* node) { _target = node; }
        
        Node* getTarget() const { return _target; }
        
        virtual void update(float t);
        
        bool isFinished() const { return _finished; }
        
    protected:
        Action();
        virtual ~Action();
        
        Node* _target;
        float _curtime;
        bool  _finished;
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
    
    
    class TintAction : public Action
    {
    public:
        static TintAction* create(const Vector4& fromColor, const Vector4& toColor, float duraton);
        
        virtual void update(float t);
        
    protected:
        TintAction();
        virtual ~TintAction();
        
        Vector4 _fromColor;
        Vector4 _toColor;
        float _duration;
    };
    
    class RotateZAction : public Action
    {
    public:
        static RotateZAction* create(float fromAngleZ, float toAngleZ, float duration);
        
        virtual void update(float t);
        
    protected:
        RotateZAction();
        virtual ~RotateZAction();
        
        float _duration;
        float _fromZ;
        float _toZ;
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
    
    class SequnceAction : public Action
    {
    public:
        static SequnceAction* create(const std::vector<Action*>& actions);

        virtual void update(float t);
        
        virtual void setTarget(Node* node);

        virtual void addRef() override;
        
        virtual void release() override;
    protected:
        SequnceAction();
        virtual ~SequnceAction();
        
        std::vector<Action*> _actions;
    };
    
    class DelayAction : public Action
    {
    public:
        static DelayAction* create(float duration);
        
        virtual void update(float t);
        
    protected:
        DelayAction();
        virtual ~DelayAction();
        
        float _duration;
    };
    
    class FrameSequnceAction : public Action
    {
    public:
        static FrameSequnceAction* create(const std::string& baseDir, int baseIdx, int count, float interval, bool repeat);
        
        virtual void update(float t);
        
    protected:
        FrameSequnceAction();
        virtual ~FrameSequnceAction();
        
        int _curFrame;
        float _accTime;
        bool  _repeat;
        float _interval;
        std::vector<Texture*> _textures;
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
