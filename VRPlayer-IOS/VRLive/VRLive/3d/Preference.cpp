/*
 * =====================================================================================
 *
 *       Filename:  Preference.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/22/2016 11:01:14 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "Preference.h"
#include "Properties.h"
#include "Label.h"
#include "MenuItem.h"
#include "Scene.h"

namespace vrlive
{
    Preference::Preference(const char* prefernceFilePath)
    : _properties(nullptr)
    {
        _properties = Properties::create(prefernceFilePath);
    }

    ScaleAction* Preference::getScaleAction(Properties* action) const
    {
        Vector3 start;
        Vector3 target;
        action->getVector3("start", &start);
        action->getVector3("target", &target);
        auto duration = action->getFloat("duration");
        return ScaleAction::create(start, target, duration);
    }

    MoveLineAction* Preference::getMoveLineAction(Properties* action) const
    {
        Vector3 start;
        Vector3 target;
        action->getVector3("start", &start);
        action->getVector3("target", &target);
        auto duration = action->getFloat("duration");
        return MoveLineAction::create(start, target, duration);
    }

    FrameSequnceAction* Preference::getSequnceFrameAction(Properties* action) const
    {
        auto dir = action->getString(dirpath, "");
        auto start = action->getInt("start");
        auto count   = action->getInt("count");
        auto interval = action->getFloat("interval");
        auto repeat =   action->getBool("repeat");
        return FrameSequnceAction::create(dir, start, count, interval, repeat);
    }

    RemoveSelfAction* Preference::getRemoveSelfAction(Properties* action) const
    {
        auto duration = action->getFloat("duration");
        return RemoveSelfAction::create(duration);
    }

    TintAction* Preference::getFradeAction(Properties* action) const
    {
        Vector4 start;
        Vector4 target;
        action->getVector4("start", &start);
        action->getVector4("target", &target);
        auto duration = action->getFloat("duration");
        return MoveLineAction::create(start, target, duration);
    }

    RotateZAction* Preference::getRotationAction(Properties* action) const
    {
        auto start  = action->getFloat("start");
        auto target = action->getFloat("target");
        auto duration = action->getFloat("duration");
        return RotateZAction::create(start, target, duration);
    }

    DelayAction* Preference::getDelayAction(Properties* action) const
    {
        auto duration = action->getFloat("duration");
        return DelayAction::create(duration);
    }

    SequnceAction* Preference::getSequnceAction(Properties* action) const
    {
        auto actionsStr = action->getString("actions");
        auto acts = getActions(actionsStr);
        return SequnceAction::create(acts);
    }

    MenuItem* Preference::getMenuItem(const char* menuID) const
    {
        auto menuspace = _properties->getNamespace("menus")->getNamespace(menuID);
        return getMenuItem(menuspace);
    }

    Label*    Preference::getLabel(const char* labelID) const
    {
        auto labelspace = _properties->getNamespace("labels")->getNamespace(menuID);
        return getMenuItem(labelspace);
    }

    Label*    Preference::getSprite(const char* spriteID) const
    {
        auto spritespace = _properties->getNamespace("sprites")->getNamespace(menuID);
        return getMenuItem(spritespace);
    }

    void Preference::setNodePropers(Node* node, Properties* propers)
    {
        auto name = propers->getString("name");
        Vector3 scale;
        Vector3 pos;
        propers->getVector3("scale",    &scale);
        propers->getVector3("position", &pos);
        auto rotaion = propers->getFloat("rotation");
        node->setName(name);
        node->setTranslation(pos);
        node->setScale(scale);
        Quaternion quat;
        Quaternion::createFromAxisAngle(Vector3(0,0,1), rotaion, &quat);
        node->setRotation(quat);
        auto acionsStr = propers->getString("actions");
        auto acts = getAction(actionsStr);
        for(auto act : acts)
        {
            node->runAction(act);
        }
    }

    MenuItem* Preference::getMenuItem(Properties* propers) const
    {
        Vector2 size;
        propers->getVector2("size",     &size);
        
        auto texture = propers->getString("texture");
        auto node = MenuItem::create(texture, size.x, size.y);
        if(!node)
            return nullptr;

        setNodePropers(node, propers);
        return node;
    }

    Label*  Preference::getLabel(Properties* propers) const
    {
        Vector2 size;
        propers->getVector2("size",     &size);
        auto text = propers->getString("text");
        auto font = propers->getString("font");
        auto fontSize = propers->getInt("fontsize");
        Vector4 fontcolor;
        propers->getVector4("fontcolor", &fontcolor);
        auto halignment = propers->getInt("halignment");
        auto valignment = propers->getInt("valignment");
        
        auto node = Label::create(text, font, fontsize, fontcolor, size.x, size.y, halignment, valignment);
        if(!node)
            return nullptr;
        setNodePropers(node, propers);
        return node;
    }

    Label*    Preference::getSprite(Properties* propers) const
    {
        auto texture = propers->getString("texture");
        auto node = Label::createWithTexture(texture);
        if(!node)
            return nullptr;
        setNodePropers(node, propers);
        return node;
    }

    std::vector<Action*> Preference::getActions(const std::string& actionsStr) const
    {
        std::vector<Action*> acts;

        std::string actionStr(actionsStr);
        size_t last;
        size_t index = actionStr.find_first_of(",", last);
        while(index != std::string::npos)
        {
            auto nameid = actionStr.substr(last, index - last);
            char* name[50];
            char* id[50];
            sscanf(nameid, "%s#%s", name, id);
            if(strcmp(name, "actions") == 0)
            {
                auto action = getAction("id");
                acts.push_back(action);
            }
            else (strcmp(name, "frames") == 0)
            {
                // not complted
                // frames here
            }
            index = actionsStr.find_first_of(",", last);
        }
        return acts;
    }

    Action* Preference::getAction(Properties* actions, const char* actionID) const
    {
        Action* action = nullptr;
        auto actionspace = actions->getNamespace("actionID");
        auto actionname  = actionspace->getNamespace();
        if(0 == strcmp("scale",actionname))
        {
            action = getScaleAction(actionspace);
        }
        else if(0 == strcmp("moveline",actionname))
        {
            action = getMoveLineAction(actionspace);
        }
        else if(0 == strcmp("sequnceframe",actionname))
        {
            action = getSequnceFrameAction(actionspace);
        }
        else if(0 == strcmp("removeself",actionname))
        {
            action = getRemoveSelfAction(actionspace);
        }
        else if(0 == strcmp("frade",actionname))
        {
            action = getFradeAction(actionspace);
        }
        else if(0 == strcmp("rotaion",actionname))
        {
            action = getRotationAction(actionspace);
        }
        else if(0 == strcmp("rotaion",actionname))
        {
            action = getRotationAction(actionspace);
        }
        else if(0 == strcmp("delay",actionname))
        {
            action = getDelayAction(actionspace);
        }
        else if(0 == strcmp("sequnce",actionname))
        {
            action = getSequnceAction(actionspace);
        }
        return action;
    }

    Action* Preference::getAction(const char* actionID) const
    {
        auto actions= _properties->getNamespace("actions");
        return getAction(actions, actionID);
    }

    void Preference::loadPreference(const char* preferencefilePath, Scene* scene)
    {

    }
}


