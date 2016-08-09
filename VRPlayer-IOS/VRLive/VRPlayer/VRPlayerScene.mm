#include <string>
#import "VRPlayerScene.h"
#import "3d/Scene.h"
#import "3d/Label.h"
#import "3d/MenuItem.h"
#import "3d/DefaultMenuItem.h"
#import "3d/Vector4.h"
#import "3d/Vector3.h"
#import "3d/Stream.h"
#import "3d/Action.h"

extern vrlive::Scene* _scene;

@interface VRPlayerScene(){
//    vrlive::Scene* scene;
}

@end
@implementation VRPlayerScene

static int s_idx = 0;

+(void) index: (int)idx
{
    s_idx = idx;
}

-(void) addLabelWithName: (NSString*)name Text: (NSString*)text NormalizedPosition:(CGPoint)position FontName:(NSString*)fontName FontColor:(UIColor*)color FontSize:(int)fontSize
{
    if (_scene)
    {
        std::string strText=[text cStringUsingEncoding: NSUTF8StringEncoding];
        std::string strFont = [fontName cStringUsingEncoding: NSUTF8StringEncoding];
        
        vrlive::Vector4 vrcolor;
#if TARGET_IPHONE_SIMULATOR
        float r,g,b,a;
#else
        double r,g,b,a;
#endif
        [color getRed:&r green:&g blue:&b alpha:&a];
        vrcolor.x = r, vrcolor.y = g, vrcolor.z = b, vrcolor.w = a;
        
        auto label = vrlive::Label::create(strText, strFont, fontSize, vrcolor);
        label->setNormalizedTranslation(position.x, position.y);
        _scene->addChild(label);
        
        float y = arc4random() % 150;
        auto action = vrlive::MoveLineAction::create(vrlive::Vector3(-500, y, 0), vrlive::Vector3(1000, y, 0), 10.f);
        
        label->runAction(action);
        action->release();
        
        auto removeAction = vrlive::RemoveSelfAction::create(10.f);
        label->runAction(removeAction);
        removeAction->release();
        
        label->release();
    }
}

- (void) addLabelWithName: (NSString*)name TexPath: (NSString*)path
{
    if (_scene)
    {
        std::string strPath = [path cStringUsingEncoding: NSUTF8StringEncoding];
        
        auto label = vrlive::Label::createWithTexture(strPath);
        label->setTranslation(vrlive::Vector3(100, 100, 0));
        _scene->addChild(label);
        
        float live = 3.f + (arc4random() % 100) / 100.f;
        
        
        {
            float halfw = _scene->getWidth() / 2;
            float halfh = _scene->getHeight() / 2;
            vrlive::Vector3 start(-halfw, halfh / 2, 0), end(0, 0, 0);
            if (s_idx == 0)
            {
                start = vrlive::Vector3(-halfw, halfh / 2, 0);
                end = vrlive::Vector3(0, 0, 0);
            }
            else if (s_idx == 1 || s_idx == 3 || s_idx == 5 || s_idx == 6)
            {
                start = end = vrlive::Vector3(0, 0, 0);
            }
            else if (s_idx == 2)
            {
                start = vrlive::Vector3(halfw, halfh/4, 0);
                end = vrlive::Vector3(-halfw, halfh * 0.8, 0);
            }
            else if (s_idx == 4)
            {
                start = vrlive::Vector3(-halfw, -halfh/4, 0);
                end = vrlive::Vector3(halfw, halfh * 0.8, 0);
            }
            else if (s_idx == 7)
            {
                start = vrlive::Vector3(-halfw, halfh * 0.4f, 0);
                end = vrlive::Vector3(0, 0, 0);
            }
            auto action = vrlive::MoveLineAction::create(start, end, live);
            
            label->runAction(action);
            action->release();
        }
        
        
        auto scaleAction = vrlive::ScaleAction::create(0.1, 1.2f + (arc4random() % 100) / 100.f * 0.5f, live);
        label->runAction(scaleAction);
        scaleAction->release();
        
        auto removeAction = vrlive::RemoveSelfAction::create(live);
        label->runAction(removeAction);
        removeAction->release();
        
        label->release();
    }
}

-(void) removeLabel: (NSString*)name
{
    if (_scene)
    {
        std::string strName=[name cStringUsingEncoding: NSUTF8StringEncoding];
        _scene->removeChild(strName);
    }
}

-(void) addMenuItemWithName: (NSString*)name TexturePath:(NSString*)path  Rect: (CGRect)rect;
{
    return;
    if (_scene)
    {
        auto menu = _scene->getDefMenuItem();
        float x = rect.origin.x;
        float y = rect.origin.y;
        float width = rect.size.width;
        float height = rect.size.height;
        
        std::string strName=[name cStringUsingEncoding: NSUTF8StringEncoding];
        std::string strPath=[path cStringUsingEncoding: NSUTF8StringEncoding];
        
        auto menuitem = vrlive::MenuItem::create(strPath, width, height);
        vrlive::Vector3 pos(0, 0, -20.f);
        menuitem->setTranslation(pos);
        _scene->addChild(menuitem);
        menuitem->setName(strName);
    }
}

-(void) removeMenuItem: (NSString*)name
{
    if (_scene)
    {
        std::string strName=[name cStringUsingEncoding: NSUTF8StringEncoding];
        _scene->getDefMenuItem()->removeChild(strName);
    }
}

@end
