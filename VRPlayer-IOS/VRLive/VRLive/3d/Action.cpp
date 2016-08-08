#include <math.h>
#include "Action.h"

namespace vrlive {
    
    void Action::update(float t)
    {
        _curtime += t;
    }
    
    Action::Action()
    : _target(nullptr)
    , _curtime(0)
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
        Vector3 pos = _start + (_end - _start) * ratio;
        _target->setTranslation(pos);
    }
    
    MoveLineAction::MoveLineAction()
    {
        
    }
    MoveLineAction::~MoveLineAction()
    {
        
    }
}
