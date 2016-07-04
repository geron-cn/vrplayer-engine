#include "EventMgr.h"
#include <algorithm>

namespace vrlive {
    EventMgr* EventMgr::s_mgr = nullptr;
    
    EventMgr* EventMgr::getInstance()
    {
        if (s_mgr == nullptr)
            s_mgr = new EventMgr();
        return s_mgr;
    }
    
    void EventMgr::postEvent(const std::string& eventname)
    {
        auto it = find(_eventNames.begin(), _eventNames.end(), eventname);
//        if (it == _eventNames.end())
        {
            _eventNames.clear();
            _eventNames.push_back(eventname);
        }
        
    }
    
    void EventMgr::removeEvent(const std::string& eventname)
    {
        for (size_t i = 0; i < _eventNames.size(); i++) {
            if (_eventNames[i] == eventname)
            {
                _eventNames.erase(_eventNames.begin() + i);
                return;
            }
        }
    }
    
    EventMgr::EventMgr()
    {
        
    }
    EventMgr::~EventMgr()
    {
        
    }
}
