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
#include "DefaultMenuItem.h"
#include "Scene.h"
#include "Action.h"
#include <vector>


namespace vrlive
{
    Preference::Preference(const char* prefernceFilePath)
        : _properties(nullptr)
        , _scene(nullptr)
        , _baseDir("")
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
        auto normalize = action->getBool("normalized");
        if(!normalize && _scene != nullptr)
        {
            auto sw  = _scene->getWidth();
            auto sh  = _scene->getHeight();
            start.x  = (start.x  - .5f) * sw;
            start.y  = (start.y  - .5f) * sh;
            target.x = (target.x - .5f) * sw;
            target.y = (target.y - .5f) * sh;
        }
        LOG("start %f %f %f ", start.x, start.y, start.z);
        LOG("target %f %f %f ", target.x, target.y, target.z);
        LOG("duration %f ", duration);
        return MoveLineAction::create(start, target, duration, normalize);
    }

    FrameSequnceAction* Preference::getSequnceFrameAction(Properties* action) const
    {
        auto dir = action->getString("dirpath", "");
        if(_baseDir != "")
            dir = (_baseDir + std::string(dir)).c_str();
        auto start = action->getInt("start");
        auto count   = action->getInt("count");
        auto interval = action->getFloat("interval");
        auto repeat =   action->getBool("repeat");
        LOG("-%s- %d %d %f", dir, start, count, interval);
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
        return TintAction::create(start, target, duration);
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
        auto labelspace = _properties->getNamespace("labels")->getNamespace(labelID);
        return getLabel(labelspace);
    }

    Label*    Preference::getSprite(const char* spriteID) const
    {
        auto spritespace = _properties->getNamespace("sprites")->getNamespace(spriteID);
        return getSprite(spritespace);
    }

    void Preference::setNodePropers(Node* node, Properties* propers) const
    {
        LOG("set properties");
        auto name = propers->getString("name");
        node->setName(name);
        Vector3 scale(1.f, 1.f, 1.f);
        Vector3 pos(.0f, .0f, 0.f);
        propers->getVector3("scale",    &scale);
        node->setScale(scale);
        if(propers->exists("normalizedpos"))
        {
            Vector2 pos2;
            propers->getVector2("normalizedpos", &pos2);
            LOG("normalizedpos %f, %f", pos2.x, pos2.y);
            node->setNormalized(pos2.x, pos2.y);
            //  if(_scene != nullptr)
            // {
            //     auto sw  = _scene->getWidth();
            //     auto sh  = _scene->getHeight();
            //     pos.x  = (pos2.x  - .5f) * sw;
            //     pos.y  = (pos2.y  - .5f) * sh;
            //     pos.z = 0;
            // }
        }
        else
        { 
            propers->getVector3("position", &pos);
            node->setTranslation(pos);
        }
        auto rotaion = propers->getFloat("rotation");
        Quaternion quat;
        Quaternion::createFromAxisAngle(Vector3(0,0,1), rotaion, &quat);
        node->setRotation(quat);
        auto actionsStr = propers->getString("actions", "");
        if(strcmp("", actionsStr) != 0 )
        {
            auto acts = getActions(actionsStr);
            for(auto act : acts)
            {
                LOG("%s run action", node->getName().c_str());
                node->runAction(act);
                act->release();
            }
        }
    }

    MenuItem* Preference::getMenuItem(Properties* propers) const
    {
        Vector2 size;
        propers->getVector2("size",     &size);
        
        auto texture = propers->getString("texture");
        if(_baseDir != "")
            texture = (_baseDir + std::string(texture)).c_str();
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
        std::string font = propers->getString("font");
        font = font.substr(font.find_first_not_of(" "));
        auto fontsize = propers->getInt("fontsize");
        Vector4 fontcolor;
        propers->getVector4("fontcolor", &fontcolor);
        auto halignment = propers->getInt("halignment");
        auto valignment = propers->getInt("valignment");
        LOG("%s %d -%s, %f,%f,%f,%f ", text, fontsize, font.c_str(), fontcolor.x, fontcolor.y, fontcolor.z, fontcolor.w);
        auto node = Label::create(text, font, fontsize, fontcolor, size.x, size.y, (TextHAlignment)halignment, (TextVAlignment)valignment);
        if(!node)
            return nullptr;
        setNodePropers(node, propers);
        return node;
    }

    Label*    Preference::getSprite(Properties* propers) const
    {
        auto texture = propers->getString("texture");
        if(_baseDir != "")
            texture = (_baseDir + std::string(texture)).c_str();
        LOG("-%s- ", texture);
        auto node = Label::createWithTexture(texture);
        if(!node)
            return nullptr;
        setNodePropers(node, propers);
        return node;
    }

    void Preference::getAction(std::string nameid, std::vector<Action*>& actions) const
    {
        LOG("get nameid %s", nameid.c_str());
        auto namepos = nameid.find_first_of("#");
        auto name = nameid.substr(0, namepos);
        name = name.substr(name.find_first_not_of(" "));
        auto id = nameid.substr(namepos +1);
        id = id.substr(0, id.find_last_not_of(" ")+1);
        LOG("get name and id -%s- -%s-", name.c_str(), id.c_str());
        //if(strcmp(name, "actions") == 0)
        if(name == "actions")
        {
            auto action = getAction(id.c_str());
            if(action != nullptr)
                actions.push_back(action);
        }
    }
    std::vector<Action*> Preference::getActions(const std::string& actionsStr) const
    {

        std::vector<Action*> acts;
        if("null" ==  actionsStr.substr(actionsStr.find_first_not_of(" ")))
            return acts;
        LOG("get actions %s", actionsStr.c_str());
        size_t last = 0;
        size_t index = actionsStr.find_first_of(",", last);
        while(index != std::string::npos)
        {
            auto nameid = actionsStr.substr(last, index - last);
            last = index + 1;
            getAction(nameid, acts);
            index = actionsStr.find_first_of(",", last);
        }
        if(index - last > 0)
        {
            auto nameid = actionsStr.substr(last, index - last);
            getAction(nameid, acts);
        }
        LOG("get actions end");
        return acts;
    }

    Action* Preference::getAction(Properties* actions, const char* actionID) const
    {
        Action* action = nullptr;
        auto actionspace = actions->getNamespace(actionID);
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

    void Preference::loadPreference(Scene* scene)
    {
        auto menus = _properties->getNamespace("menus");
        _scene = scene;
        Properties* proper = NULL;
        if(menus)
        while((proper = menus->getNextNamespace()) != NULL)
        {
            if(0 == strcmp("menu", proper->getNamespace()))
            {
                LOG("load menu %s", proper->getId());
                auto menu = getMenuItem(proper);
                scene->getDefMenuItem()->addChild(menu);
                menu->release();
            }
        }

        auto labels = _properties->getNamespace("labels");
        if(labels)
        while((proper = labels->getNextNamespace()) != NULL)
        {
            if(0 == strcmp("label", proper->getNamespace()))
            {
                LOG("load label %s", proper->getId());
                auto label = getLabel(proper);
                scene->addChild(label);
                label->release();
            }
        }

        auto sprites = _properties->getNamespace("sprites");
        if(sprites)
        while((proper = sprites->getNextNamespace()) != NULL)
        {
            if(0 == strcmp("sprite", proper->getNamespace()))
            {
                LOG("load sprite %s", proper->getId());
                auto sprite = getSprite(proper);
                scene->addChild(sprite);
                sprite->release();
            }
        }
    }

   void Preference::loadPreference(const char* preferencefilePath, Scene* scene)
   {
       auto prefern = new Preference(preferencefilePath);
       prefern->loadPreference(scene);

        std::string dir(preferencefilePath);
        auto pos = dir.find_last_of("/");
        if(pos != std::string::npos)
            prefern->_baseDir = dir.substr(0, pos + 1);

       delete prefern;
   }

    Preference::~Preference()
    {
        if(_properties)
        {
            delete _properties;
            _properties = nullptr;
        }
    }
}


