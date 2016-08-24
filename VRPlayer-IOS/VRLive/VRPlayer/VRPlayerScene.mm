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
#import "3d/Sprite3D.h"

extern vrlive::Scene* _scene;

@interface VRPlayerScene(){
//    vrlive::Scene* scene;
}

@end
@implementation VRPlayerScene

-(void) addLabelWithName: (NSString*)name Text: (NSString*)text FontName:(NSString*)fontName FontColor:(UIColor*)color FontSize:(int)fontSize  NormalizedY:(float)y Duration:(float)duration
{
    if (_scene)
    {
        std::string strText=[text cStringUsingEncoding: NSUTF8StringEncoding];
        std::string strFont = [fontName cStringUsingEncoding: NSUTF8StringEncoding];
        
        vrlive::Vector4 vrcolor;
        CGFloat r, g, b, a;
        
        [color getRed:&r green:&g blue:&b alpha:&a];
        vrcolor.x = r, vrcolor.y = g, vrcolor.z = b, vrcolor.w = a;
        
        auto label = vrlive::Label::create(strText, strFont, fontSize, vrcolor);
        int w = _scene->getWidth();
        int h = _scene->getHeight();
        y = h * y - h / 2;
        float x = w / 2.f + label->getWidth() / 2.f;
        label->setTranslation(vrlive::Vector3(-x, y, 0));
        _scene->addChild(label);
        
        auto action = vrlive::MoveLineAction::create(vrlive::Vector3(-x, y, 0), vrlive::Vector3(x, y, 0), duration);
    
        label->runAction(action);
        action->release();
        
        auto removeAction = vrlive::RemoveSelfAction::create(duration);
        label->runAction(removeAction);
        removeAction->release();
        
        label->release();
    }
}

- (void) addLabelWithName2: (NSString*)name TexDir: (NSString*)dir BaseIndex: (int)baseIndex FrameCount: (int)framecount NormalizedStart: (CGPoint)start NormalizedEnd: (CGPoint)end Duration:(float)duration
{
    if (_scene)
    {
        std::string strDir = [dir cStringUsingEncoding: NSUTF8StringEncoding];
        char str[512];
        sprintf(str, "%s%d%s", strDir.c_str(), baseIndex, ".png");
        auto label = vrlive::Label::createWithTexture(str);
        _scene->addChild(label);
        label->setTranslation(vrlive::Vector3(0, 0, 0));
        auto action = vrlive::FrameSequnceAction::create(strDir, baseIndex, framecount, 0.2f, true);
        label->runAction(action);
        
        float live = duration;//4.f + (arc4random() % 100) / 100.f;
        float w = _scene->getWidth();
        float h = _scene->getHeight();
        vrlive::Vector3 vrstart((start.x - 0.5f) * w, (start.y - 0.5f) * h, 0), vrend((end.x - 0.5f) * w, (end.y - 0.5f) * h, 0);
        
        auto moveAction = vrlive::MoveLineAction::create(vrstart, vrend, live);
        label->runAction(moveAction);
        auto removeAction = vrlive::RemoveSelfAction::create(live);
        label->runAction(removeAction);
        removeAction->release();
        
        label->release();
    }
}

- (void) addTextureLabelWithName: (NSString*)name TexPath: (NSString*)path NormalizedStart: (CGPoint)start NormalizedEnd: (CGPoint)end Duration:(float)duration FadeInAndOut:(BOOL) useFade
{
    if (_scene)
    {
        std::string strPath = [path cStringUsingEncoding: NSUTF8StringEncoding];
        
        auto label = vrlive::Label::createWithTexture(strPath);
        label->setTranslation(vrlive::Vector3(100, 100, 0));
        _scene->addChild(label);
        
        float live = duration;
        
        
        {
            float w = _scene->getWidth();
            float h = _scene->getHeight();
            vrlive::Vector3 vrstart, vrend;
            vrstart.x = (start.x - 0.5f) * w;
            vrstart.y = (start.y - 0.5f) * h;
            vrstart.z = 0;
            vrend.x = (end.x - 0.5f) * w;
            vrend.y = (end.y - 0.5f) * h;
            vrend.z = 0;
            
            auto action = vrlive::MoveLineAction::create(vrstart, vrend, live);
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
        
        if (useFade)
        {
            auto tint1 = vrlive::TintAction::create(vrlive::Vector4(1.f, 1.f, 1.f, 0.f), vrlive::Vector4(1.f, 1.f, 1.f, 1.f), live * 0.5f);
            auto tint2 = vrlive::TintAction::create(vrlive::Vector4(1.f, 1.f, 1.f, 1.f), vrlive::Vector4(1.f, 1.f, 1.f, 0.f), live * 0.5);
            
            std::vector<vrlive::Action*> acts;
            acts.push_back(tint1);
            acts.push_back(tint2);
            auto sq = vrlive::SequnceAction::create(acts);
            label->runAction(sq);
            sq->release();
        }
//        if (s_idx == 6)
//        {
//            std::vector<vrlive::Vector3> pos;
//            float x = _scene->getWidth() / 8;
//            float y = _scene->getHeight() / 8;
//            pos.push_back(vrlive::Vector3(x, y, 0));
//            pos.push_back(vrlive::Vector3(-x, y, 0));
//            pos.push_back(vrlive::Vector3(x, -y, 0));
//            pos.push_back(vrlive::Vector3(-x, -y, 0));
//            
//            for (size_t k = 0; k < pos.size(); k++)
//            {
//                label = vrlive::Label::createWithTexture(strPath);
//                
//                label->setTranslation(vrlive::Vector3(pos[k].x, pos[k].y, 0));
//                auto tint1 = vrlive::TintAction::create(vrlive::Vector4(1.f, 1.f, 1.f, 0.f), vrlive::Vector4(1.f, 1.f, 1.f, 0.f), live * 0.5f);
//                auto tint2 = vrlive::TintAction::create(vrlive::Vector4(1.f, 1.f, 1.f, 0.f), vrlive::Vector4(1.f, 1.f, 1.f, 1.f), live * 0.5);
//                
//                std::vector<vrlive::Action*> acts;
//                acts.push_back(tint1);
//                acts.push_back(tint2);
//                auto sq = vrlive::SequnceAction::create(acts);
//                label->runAction(sq);
//                sq->release();
//                auto removeAction = vrlive::RemoveSelfAction::create(live);
//                label->runAction(removeAction);
//                removeAction->release();
//                _scene->addChild(label);
//                label->release();
//            }
//            
//        }
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
        vrlive::Vector3 pos(x, y, -20.f);
        menuitem->setTranslation(pos);
        menu->addChild(menuitem);
        menuitem->setName(strName);
        menuitem->release();
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

- (void) showPlayerHeadControlMenu: (BOOL)show
{
    if (_scene)
    {
        _scene->getDefMenuItem()->showPlayerMenu(show);
    }
}

- (void) showPlayerCustomMenu: (BOOL)show
{
    if (_scene)
    {
        _scene->getDefMenuItem()->setCustomMenuShow(show);
    }
}

- (void) showCusor: (BOOL)show
{
    if (_scene)
    {
        _scene->setCursorVisible(show);
    }
}

+ (void) labelRotation:(int)degree
{
    vrlive::Label::rotationZ(degree);
}

@end
