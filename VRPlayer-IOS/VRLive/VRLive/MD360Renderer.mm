//
//  MD360Renderer.m
//  MD360Player4IOS
//
//  Created by ashqal on 16/4/7.
//  Copyright © 2016年 ashqal. All rights reserved.
//
//#include <vector>

#import "MD360Renderer.h"
#import "MDAbsObject3D.h"
#import "MD360Program.h"
#import "GLUtil.h"
#import "MD360Director.h"
#import "3d/Scene.h"
#import "3d/Camera.h"
#import "3d/EventMgr.h"
#import "3d/Texture.h"
#import "ijkmedia/ijksdl/ios/IJKSDLGLView.h"
#include <vector>
#include <string>

@interface MD360Renderer()
{
vrlive::Scene* _scene;
std::string _postedEvent;
    vrlive::Texture* _texture;
}

@property (nonatomic,strong) MDAbsObject3D* mObject3D;
@property (nonatomic,strong) MD360Program* mProgram;
@property (nonatomic,strong) MD360Texture* mTexture;
@property (nonatomic,strong) NSArray* mDirectors;
@property (nonatomic,strong) MDDisplayStrategyManager* mDisplayStrategyManager;
@end

@implementation MD360Renderer



+ (MD360RendererBuilder*) builder{
    return [[MD360RendererBuilder alloc]init];
}

- (instancetype)init {
    self = [super init];
    if (self) {
        [self setup];
    }
//    _postedEvent = "";
    _scene = nullptr;
    _texture = nullptr;
    
    return self;
}

- (void)dealloc{
    if (_scene)
    {
        _scene->release();
        _scene = nullptr;
        vrlive::EventMgr::getInstance()->clearEvents();
    }
    if (_texture)
    {
        _texture->release();
        _texture = nullptr;
    }
    [self.mObject3D destroy];
    [self.mProgram destroy];
    self.mDirectors = nil;
}

- (void) setup{
    self.mProgram = [[MD360Program alloc]init];
}

- (void) rendererOnCreated:(EAGLContext*)context{
    [GLUtil glCheck:@"glEnable"];
    
    // init
    [self initProgram];
    [GLUtil glCheck:@"initProgram"];
//
//    [self initTexture:context];
//    [GLUtil glCheck:@"initTexture"];
    
    [self initObject3D];
    [GLUtil glCheck:@"initObject3D"];
    
    _scene = vrlive::Scene::create();
    vrlive::EventMgr::getInstance()->clearEvents();
}

- (void) rendererOnChanged:(EAGLContext*)context width:(int)width height:(int)height{
    // update surface
//    [self.mTexture resize:width height:height];
    //setup camera
    if (_scene)
    {
        vrlive::Camera* camera = vrlive::Camera::createPerspective(60, (float)width/(float)height, 0.1f, 100.f);
        _scene->setCamera(camera);
        
        camera->release();
    }
}

- (void) rendererOnDrawFrame:(EAGLContext*)context width:(int)width height:(int)height{
    
    // clear
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    [GLUtil glCheck:@"glClear"];
    
  
    
    float scale = [GLUtil getScrrenScale];
    int widthPx = width * scale;
    int heightPx = height * scale;
    
    int size = [self.mDisplayStrategyManager getVisibleSize];
    int itemWidthPx = widthPx * 1.0 / size;
    
    
    for (int i = 0; i < size; i++ ) {
        if (i >= [self.mDirectors count]) {
            return;
        }
        // use
        [self.mProgram use];
        [GLUtil glCheck:@"mProgram use"];
        
        //    [[IJKSDLGLView instance] updateTextureAndProgram:]
        
        // update texture
        //    [self.mTexture updateTexture:context];
        
        
        MD360Director* direcotr = [self.mDirectors objectAtIndex:i];
        glViewport(itemWidthPx * i, 0, itemWidthPx, heightPx);

        // Update Projection
        [direcotr updateProjection:itemWidthPx height:heightPx];
        IJKSDLGLView* instance = [IJKSDLGLView instance];
        if (instance)
        {
//            GLKMatrix4 mvp = [ [self.mDirectors objectAtIndex:i] getMVPMatrix];//
            if ([instance tryLockTextureData])
            {
                int w = [instance getTextureWidth];
                int h = [instance getTextureHight];
                if (_texture && (_texture->getWidth() != w || _texture->getHeight() != h))
                {
                    _texture->release();
                    _texture = nullptr;
                }
                unsigned char* data = [instance popTextureData];
                if (data)
                {
                    if (_texture == nullptr)
                    {
                        _texture = vrlive::Texture::create(vrlive::Texture::Format::RGB, w, h, data);
                    }
                    else
                        _texture->setTextureData(data);
                }
                
                [instance unLockTextureData];
            }
            
//            [instance updateTextureAndProgram:mvp.m posbuffer:[self.mObject3D getVertexBuffer] texbuffer:[self.mObject3D getTextureBuffer]];
        }
        if (_texture)
        {
            _texture->bind();
        }
//        if ([self.mObject3D getIndices] != nil) {
//            glDrawElements(GL_TRIANGLES, self.mObject3D.mNumIndices, GL_UNSIGNED_SHORT, [self.mObject3D getIndices]);
//        } else {
//            glDrawArrays(GL_TRIANGLES, 0, self.mObject3D.mNumIndices);
//        }
//        [GLUtil glCheck:@"glDrawArrays"];
        
        // Pass in the combined matrix.
        [direcotr shot:self.mProgram];
        [GLUtil glCheck:@"shot"];
        
        // Tell the texture uniform sampler to use this texture in the shader by binding to texture unit 0.
        glUniform1i(self.mProgram.mTextureUniformHandle, 0);
        [GLUtil glCheck:@"glUniform1i mTextureUniformHandle"];
        
        // upload
        [self.mObject3D uploadDataToProgram:self.mProgram];
        [GLUtil glCheck:@"uploadDataToProgram"];

        [self.mObject3D onDraw];
        
        
    }
    
    for (int i = 0; i < size; i++) {
        if (i >= [self.mDirectors count]) {
            return;
        }
        
        MD360Director* direcotr = [self.mDirectors objectAtIndex:i];
        glViewport(itemWidthPx * i, 0, itemWidthPx, heightPx);
        //draw scene
        if (_scene)
        {
            GLKMatrix4 mat = [direcotr getCurrentRotation];
            
            //        vrlive::Vector3 dir = vrlive::Vector3(mat.m[8], mat.m[9], mat.m[10]);
            //
            //        NSLog(@"%.2f,%.2f,%.2f", dir.x, dir.y, dir.z);
            
            auto camera = _scene->getCamera();
            camera->setViewMatrix(mat.m);
            _scene->draw();
        }
    }
    
    auto events = vrlive::EventMgr::getInstance()->getEvents();
    if (events.size() == 0)
        _postedEvent = "";
    else if (_postedEvent != events[0])
    {
        
        _postedEvent = events[0];
        NSString *str= [NSString stringWithCString:_postedEvent.c_str() encoding:[NSString defaultCStringEncoding]];
        [[NSNotificationCenter defaultCenter] postNotificationName:str object:nil];
        
        
    }
}

- (void) initProgram {
    [self.mProgram build];
}

- (void) initTexture:(EAGLContext*)context {
    [self.mTexture createTexture:context];
}

- (void) initObject3D {
    // load
    [self.mObject3D loadObj];
  
       
}

- (void) rendererOnDestroy:(EAGLContext*) context{
//    [self.mObject3D destroy];
//    [self.mProgram destroy];
//    self.mDirectors = nil;
}

@end

@interface MD360RendererBuilder()
@property (nonatomic,readonly) NSArray* directors;
@property (nonatomic,readonly) MD360Texture* texture;
@property (nonatomic,readonly) MDAbsObject3D* object3D;
@property (nonatomic,readonly) MDDisplayStrategyManager* displayStrategyManager;
@end

@implementation MD360RendererBuilder

- (void) setDirectors:(NSArray*) directors{
    _directors = directors;
}

- (void) setTexture:(MD360Texture*) texture{
    _texture = texture;
}

- (void) setObject3D:(MDAbsObject3D*) object3D{
    _object3D = object3D;
}

- (void) setDisplayStrategyManager:(MDDisplayStrategyManager*) displayStrategyManager{
    _displayStrategyManager = displayStrategyManager;
}

- (MD360Renderer*) build{
    MD360Renderer* renderer = [[MD360Renderer alloc]init];
    renderer.mDirectors = self.directors;
    renderer.mTexture = self.texture;
    renderer.mObject3D = self.object3D;
    renderer.mDisplayStrategyManager = self.displayStrategyManager;
    return renderer;
}

@end
