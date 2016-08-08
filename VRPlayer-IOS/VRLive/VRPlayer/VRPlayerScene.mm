#include <string>
#import "VRPlayerScene.h"
#import "3d/Scene.h"
#import "3d/Label.h"
#import "3d/MenuItem.h"
#import "3d/DefaultMenuItem.h"
#import "3d/Vector4.h"
#import "3d/Vector3.h"
#import "3d/Stream.h"

extern vrlive::Scene* _scene;

@interface VRPlayerScene(){
//    vrlive::Scene* scene;
}

@end
@implementation VRPlayerScene

-(void) addLabel: (NSString*)name Text: (NSString*)text NormalizedPosition:(CGPoint)position FontName:(NSString*)fontName FontColor:(UIColor*)color FontSize:(int)fontSize
{
    if (_scene)
    {
        std::string strText=[text cStringUsingEncoding: NSUTF8StringEncoding];
        std::string strFont = [fontName cStringUsingEncoding: NSUTF8StringEncoding];
        
        vrlive::Vector4 vrcolor;
        [color getRed:&vrcolor.x green:&vrcolor.y blue:&vrcolor.z alpha:&vrcolor.w];
        
        auto label = vrlive::Label::create(strText, strFont, fontSize, vrcolor);
        label->setNormalizedTranslation(position.x, position.y);
        _scene->addChild(label);
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

-(void) addMenuItem: (NSString*)name TexturePath:(NSString*)path  Rect: (CGRect)rect;
{
    if (_scene)
    {
        auto menu = _scene->getDefMenuItem();
        float x = rect.origin.x;
        float y = rect.origin.y;
        float width = rect.size.width;
        float height = rect.size.height;
        
        auto stream = vrlive::MemoryStream::create(nullptr, 0);
        auto menuitem = vrlive::MenuItem::create(stream, width, height);
        vrlive::Vector3 pos(x, y, -20.f);
        menuitem->setTranslation(pos);
        menu->addChild(menuitem);
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
