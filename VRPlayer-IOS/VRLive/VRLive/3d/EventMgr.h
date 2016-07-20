//
// Created by tony
//
#ifndef EVENT_MGR_H_
#define EVENT_MGR_H_
#include <vector>
#include <string>

namespace vrlive {
    
    class EventMgr
    {
    public:
        static EventMgr* getInstance();
        
        void postEvent(const std::string& eventname);
        
        void clearEvents() { _eventNames.clear(); }
        
        const std::vector<std::string>& getEvents() const { return _eventNames; }
        
        void removeEvent(const std::string& eventname);
        
    protected:
        EventMgr();
        ~EventMgr();
        
        std::vector<std::string> _eventNames;

        static EventMgr* s_mgr;
    };
}

#endif
