#include <math.h>
#include "Platform.h"
#include "Action.h"
#include "Sprite3D.h"
#include "Quaternion.h"

namespace vrlive {
    
    void Action::update(float t)
    {
        _curtime += t;
    }
    
    Action::Action()
    : _target(nullptr)
    , _curtime(0)
    , _finished(false)
    {
        
    }
    Action::~Action()
    {
        
    }
    
    ///////////////////////////////////////////////////
    MoveLineAction* MoveLineAction::create(const Vector3& start, const Vector3& end, float t)
    {
        auto action = new MoveLineAction();
        
        action->_start = start;
        action->_end = end;
        action->_duration = t;
        
        return action;
    }
    
    void MoveLineAction::update(float t)
    {
        Action::update(t);
        
        float ratio = _curtime / _duration;
        if (ratio >= 1.f)
        {
            _finished = true;
            ratio = 1.f;
        }
        Vector3 pos = _start + (_end - _start) * ratio;
        _target->setTranslation(pos);
    }
    
    MoveLineAction::MoveLineAction()
    {
        
    }
    MoveLineAction::~MoveLineAction()
    {
        
    }
    
    RemoveSelfAction* RemoveSelfAction::create(float duration)
    {
        auto action = new RemoveSelfAction();
        action->_duration = duration;
        return action;
    }
    
    void RemoveSelfAction::update(float t)
    {
        Action::update(t);
        if (_curtime >= _duration)
        {
            auto parent = _target->getParent();
            parent->removeChild(_target);
        }
    }
    
    RemoveSelfAction::RemoveSelfAction()
    : _duration(1.f)
    {
        
    }
    RemoveSelfAction::~RemoveSelfAction()
    {
        
    }
    
    ScaleAction* ScaleAction::create(float scaleFrom, float scaleTo, float duration)
    {
        auto action = new ScaleAction();
        action->_scaleFrom = scaleFrom;
        action->_scaleTo = scaleTo;
        action->_duration = duration;
        
        return action;
    }
    
    void ScaleAction::update(float t)
    {
        Action::update(t);
        float ratio = _curtime / _duration;
        if (ratio >= 1.f)
        {
            _finished = true;
            ratio = 1.f;
        }
        if (_target)
        {
            float s = _scaleFrom + (_scaleTo - _scaleFrom) * ratio;
            _target->setScale(Vector3(s,s,s));
        }
    }
    
    ScaleAction::ScaleAction()
    : _scaleFrom(1.f)
    , _scaleTo(1.f)
    , _duration(1.f)
    {
        
    }
    ScaleAction::~ScaleAction()
    {
        
    }
    
    TintAction* TintAction::create(const Vector4& fromColor, const Vector4& toColor, float duraton)
    {
        auto action = new TintAction;
        action->_duration = duraton;
        action->_fromColor = fromColor;
        action->_toColor = toColor;
        
        return action;
    }
    
    void TintAction::update(float t)
    {
        Action::update(t);
        float ratio = _curtime / _duration;
        if (ratio >= 1.f)
        {
            _finished = true;
            ratio = 1.f;
        }
        Vector4 color;
        color = _fromColor + (_toColor - _fromColor) * ratio;
        auto sprite = _target->getSprite3D();
        if (sprite)
            sprite->setColor(color);
    }
    
    TintAction::TintAction()
    : _fromColor(1.f, 1.f, 1.f, 1.f)
    , _toColor(1.f, 1.f, 1.f, 1.f)
    , _duration(1.f)
    {
        
    }
    TintAction::~TintAction()
    {
        
    }
    
    RotateZAction* RotateZAction::create(float fromAngleZ, float toAngleZ, float duration)
    {
        auto action = new RotateZAction();
        action->_duration = duration;
        action->_fromZ = fromAngleZ;
        action->_toZ = toAngleZ;
        return action;
    }
    
    void RotateZAction::update(float t)
    {
        Action::update(t);
        float ratio = _curtime / _duration;
        if (ratio >= 1.f)
        {
            _finished = true;
            ratio = 1.f;
        }
        float angle = _fromZ + (_toZ - _fromZ) * ratio;
        angle = angle * 3.1415926f / 180.f;
        
        Quaternion quat;
        Quaternion::createFromAxisAngle(Vector3(0,0,1), angle, &quat);
        
        _target->setRotation(quat);
    }
    
    RotateZAction::RotateZAction()
    : _fromZ(0)
    , _toZ(0)
    , _duration(1.f)
    {
        
    }
    RotateZAction::~RotateZAction()
    {
        
    }
    
    /////////////////////////////////////////////////////////////////
    ActionMgr* ActionMgr::_instance = nullptr;
    ActionMgr* ActionMgr::getInstance()
    {
        if (_instance == nullptr)
        {
            _instance = new ActionMgr();
        }
        return _instance;
    }
    
    void ActionMgr::destroyInstance()
    {
        if (_instance)
        {
            delete _instance;
            _instance = nullptr;
        }
    }
    
    void ActionMgr::init()
    {
        _lasttime = Platform::getMachTimeInMilliseconds();
    }
    
    void ActionMgr::addAction(Action* action)
    {
        for (auto it : _actions) {
            if (it == action)
                return;
        }
        
        if (action)
        {
            action->addRef();
            _actions.push_back(action);
        }
    }
    
    void ActionMgr::removeActionByNode(Node* node)
    {
        for (size_t i = 0; i < _actions.size(); i++) {
            auto action = _actions[i];
            if (action->getTarget() == node)
            {
                action->release();
                _actions.erase(_actions.begin() + i);
                i--;
            }
        }
    }
    
    void ActionMgr::removeAction(Action* action)
    {
        for (size_t i = 0; i < _actions.size(); i++) {
            if (action == _actions[i])
            {
                action->release();
                _actions.erase(_actions.begin() + i);
                i--;
            }
        }
    }
    
    void ActionMgr::update()
    {
        double time = Platform::getMachTimeInMilliseconds();
        double t = (time - _lasttime) / 1000.0;
        for (auto action : _actions) {
            if (action)
            {
                action->update(t);
            }
        }
        _lasttime = time;
    }
    
    ActionMgr::ActionMgr()
    : _lasttime(0.0)
    {
        
    }
    ActionMgr::~ActionMgr()
    {
        for (auto action : _actions) {
            if (action)
            {
                action->release();
            }
        }
    }
}
