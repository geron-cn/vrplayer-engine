#include <math.h>
#include "Platform.h"
#include "Action.h"
#include "Sprite3D.h"
#include "Quaternion.h"

#include "../FileUtils/FileUtils.h"

namespace vrlive {
    
    void Action::update(float t)
    {
        _curtime += t;
        LOG("action updated %s", _target->getName().c_str());
    }
    
    Action::Action()
    : _target(nullptr)
    , _curtime(0)
    , _finished(false)
    {
        
    }
    Action::~Action()
    {
        LOG("action destruct");
        _target = nullptr;
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
        
        if(_target == nullptr)
            return;

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
        LOG("~MoveLineAction");
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

        if(_target == nullptr)
            return;

        if (_curtime >= _duration)
        {
            _finished = true;
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
        action->_scaleFrom = Vector3(scaleFrom, scaleFrom, scaleFrom);
        action->_scaleTo = Vector3(scaleTo, scaleTo, scaleTo);
        action->_duration = duration;
        
        return action;
    }

    ScaleAction* ScaleAction::create(vrlive::Vector3 scaleFrom, vrlive::Vector3 scaleTo, float duration)
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
            float sx = _scaleFrom.x + (_scaleTo.x - _scaleFrom.x) * ratio;
            float sy = _scaleFrom.y + (_scaleTo.y - _scaleFrom.y) * ratio;
            float sz = _scaleFrom.z + (_scaleTo.z - _scaleFrom.z) * ratio;
            _target->setScale(Vector3(sx,sy,sz));
        }
    }
    
    ScaleAction::ScaleAction()
    : _scaleFrom(1.f, 1.f, 1.f)
    , _scaleTo(1.f, 1.f, 1.f)
    , _duration(1.f)
    {
        
    }
    ScaleAction::~ScaleAction()
    {
        LOG("~ScaleAction");
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
    
    FrameSequnceAction* FrameSequnceAction::create(const std::string& baseDir, int baseIdx, int count, float interval, bool repeat)
    {
        auto action = new FrameSequnceAction();
        action->_repeat = repeat;
        action->_accTime = 0;
        action->_curFrame = 0;
        action->_interval = interval;
        
        
        char str[512];
        for (int i = 0; i < count; i++) {
            sprintf(str, "%s%d%s", baseDir.c_str(), i + baseIdx, ".png");
            auto tex = Texture::create(str);
            action->_textures.push_back(tex);
        }
        
        return action;
    }
    
    void FrameSequnceAction::update(float t)
    {
        Action::update(t);
        _curFrame = (int)(_curtime / _interval);
        auto sprite = _target->getSprite3D();
        if (_curFrame >= _textures.size())
        {
            if (_repeat)
                _curFrame %= _textures.size();
            else
            {
                _finished = true;
                return;
            }
        }
        if (sprite && sprite->getTexture() != _textures[_curFrame])
        {
            sprite->setTexture(_textures[_curFrame]);
        }
    }
    
    FrameSequnceAction::FrameSequnceAction()
    : _curFrame(0)
    , _accTime(0)
    , _repeat(true)
    {
        
    }
    FrameSequnceAction::~FrameSequnceAction()
    {
        for (auto it : _textures) {
            it->release();
        }
        _textures.clear();
    }
    
    SequnceAction* SequnceAction::create(const std::vector<Action*>& actions)
    {
        auto action = new SequnceAction();
        action->_actions = actions;
        return action;
    }
    
    void SequnceAction::setTarget(Node* node)
    {
        Action::setTarget(node);
        for (auto it : _actions) {
            it->setTarget(node);
        }
    }
    
    void SequnceAction::update(float t)
    {
        Action::update(t);
        
        for (auto it : _actions)
        {
            if (!it->isFinished())
            {
                it->update(t);
                return;
            }
        }
        _finished = true;
    }

    void SequnceAction::addRef()
    {
        Ref::addRef();
        for( auto action : _actions)
        {
            action->addRef();
        }
    }

    void SequnceAction::release()
    {
        Ref::release();
        for(auto action : _actions)
        {
            action->release();
        }
    }
    
    SequnceAction::SequnceAction()
    {
        
    }

    SequnceAction::~SequnceAction()
    {
        LOG("~SequnceAction");
        for (auto iter :_actions)
        {
            iter->setTarget(nullptr);
        }
        _actions.clear();
    }

    DelayAction* DelayAction::create(float duration)
    {
        auto action = new DelayAction();
        action->_duration = duration;
        
        return action;
    }
    
    void DelayAction::update(float t)
    {
        Action::update(t);
        if (_curtime >= _duration)
            _finished = true;
    }
    
    DelayAction::DelayAction()
    {
        
    }
    DelayAction::~DelayAction()
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
        int len = _actions.size() - 1;
        for(int i=len; i>=0; i--)
        {
            auto action = _actions[i];
            if (action->getTarget() == node)
            {
                 LOG("action remove from manager %s", node->getName().c_str());
                action->setTarget(nullptr);
//                action->release();
//                _actions.erase(_actions.begin() + i);
                LOG("action remove from manager ended ");
            }
        }
    }
    
    void ActionMgr::removeAction(Action* action)
    {
        int len = _actions.size() - 1;
        for(int i=len; i>=0; i--)
        {
            if (action == _actions[i])
            {
//                _actions.erase(_actions.begin() + i);
                action->setTarget(nullptr);
//                action->release();
            }
        }
    }
    
    void ActionMgr::update()
    {
        LOG("action mgr size: %d", _actions.size());
        double time = Platform::getMachTimeInMilliseconds();
        double t = (time - _lasttime) / 1000.0;
        for (auto action : _actions) {
            if (action && action->getTarget())
            {
                action->update(t);
            }
        }
        
        for (size_t i = 0; i < _actions.size(); i++) {
            if (_actions[i]->getTarget() == nullptr)
            {
                _actions.erase(_actions.begin() + i);
                i--;
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
