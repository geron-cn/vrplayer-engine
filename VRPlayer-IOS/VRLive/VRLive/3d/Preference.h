/*
 * =====================================================================================
 *
 *       Filename:  Preference.h
 *
 *    Description:  read prefernce file to objects and actions
 *
 *        Version:  1.0
 *        Created:  08/22/2016 11:00:29 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __PREFERENDCE_H__
#define __PREFERENDCE_H__

#include "Action.h"

namespace vrlive
{
    class Properties;
    class Label;
    class MenuItem;
    class Scene;

class Preference
{
public:

    static void loadPreference(const char* preferencefilePath, Scene* scene);

    Preference(const char* preferenceFilePath);
    MenuItem* getMenuItem(const char* menuID) const;
    Label*    getLabel(const char* labelID) const;
    Label*    getSprite(const char* spriteID) const;

    // get action by id, 
    // actions has been binded(node->runAction) to nodes, no need to call this
    Action*   getAction(const char* actionID) const;

    virtual ~Preference();
private:
    Preference():_properties(nullptr){}

    void loadPreference(Scene* scene) const;

    MenuItem* getMenuItem(Properties* propers) const;
    Label*    getLabel(Properties* propers) const;
    Label*    getSprite(Properties* propers) const;
    void      setNodePropers(Node* node, Properties* propers) const;

    Action*   getAction(Properties* actions, const char* actionID) const;
    std::vector<Action*> getActions(const std::string& actionsStr) const;

    ScaleAction*    getScaleAction(Properties* action) const;
    MoveLineAction* getMoveLineAction(Properties* action) const;
    FrameSequnceAction* getSequnceFrameAction(Properties* action) const;
    RemoveSelfAction*   getRemoveSelfAction(Properties* action) const;
    TintAction*    getFradeAction(Properties* action) const;
    RotateZAction* getRotationAction(Properties* action) const;
    DelayAction*   getDelayAction(Properties* action) const;
    SequnceAction* getSequnceAction(Properties* action) const;

    Properties* _properties;
};

}

#endif // __PREFERENDCE_H__
