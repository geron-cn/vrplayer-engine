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
#import "3d/Scene.h"
#import "3d/Camera.h"
#import "3d/EventMgr.h"
#include <vector>
#include <string>

@interface MD360Renderer()
{
vrlive::Scene* _scene;
std::string _postedEvent;
}
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
    return self;
}

- (void)dealloc{
    [self.mObject3D destroy];
    [self.mProgram destroy];
    [self.mDirector destroy];
    if (_scene)
    {
        _scene->release();
        _scene = nullptr;
        vrlive::EventMgr::getInstance()->clearEvents();
    }
}

- (void) setup{
    self.mProgram = [[MD360Program alloc]init];
    self.mObject3D = [[MDSphere3D alloc]init];
    
}

- (void) rendererOnCreated:(EAGLContext*)context{
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_TEXTURE_2D);
    //glEnable(GL_BLEND);
    [GLUtil glCheck:@"glEnable"];
    
    // init
    [self initProgram];
    [GLUtil glCheck:@"initProgram"];
    
    [self initTexture];
    [GLUtil glCheck:@"initTexture"];
    
    [self initObject3D];
    [GLUtil glCheck:@"initObject3D"];
    
    _scene = vrlive::Scene::create();
    vrlive::EventMgr::getInstance()->clearEvents();
}

- (void) rendererOnChanged:(EAGLContext*)context width:(int)width height:(int)height{
    // Set the OpenGL viewport to the same size as the surface.
    glViewport(0, 0, width, height);
    
    // update surface
    [self.mTexture resize:width height:height];
    
    // Update Projection
    [self.mDirector updateProjection:width height:height];
    
    //setup camera
    if (_scene)
    {
        vrlive::Camera* camera = vrlive::Camera::createPerspective(60, (float)width/(float)height, 0.1f, 100.f);
        _scene->setCamera(camera);
        
        camera->release();
    }
    
}

- (void) rendererOnDrawFrame:(EAGLContext*)context{
    // clear
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);
    [GLUtil glCheck:@"glClear"];
    
    // use
    [self.mProgram use];
    [GLUtil glCheck:@"mProgram use"];
    
    // update texture
    [self.mTexture updateTexture:context];
    
    // Tell the texture uniform sampler to use this texture in the shader by binding to texture unit 0.
    glUniform1i(self.mProgram.mTextureUniformHandle, 0);
    [GLUtil glCheck:@"glUniform1i mTextureUniformHandle"];
    
    // Pass in the combined matrix.
    [self.mDirector shot:self.mProgram];
    [GLUtil glCheck:@"shot"];
    
    if ([self.mObject3D getIndices] != nil) {
        [self.mObject3D uploadDataToProgram:self.mProgram];
        glDrawElements(GL_TRIANGLES, self.mObject3D.mNumIndices, GL_UNSIGNED_SHORT, [self.mObject3D getIndices]);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, self.mObject3D.mNumIndices);
    }
    // Draw
    
    [GLUtil glCheck:@"glDrawArrays"];
    [self.mProgram unuse];

    //draw scene
    if (_scene)
    {
        GLKMatrix4 mat = [self.mDirector getCurrentRotation];
        
//        vrlive::Vector3 dir = vrlive::Vector3(mat.m[8], mat.m[9], mat.m[10]);
//        
//        NSLog(@"%.2f,%.2f,%.2f", dir.x, dir.y, dir.z);
        
        auto camera = _scene->getCamera();
        camera->setViewMatrix(mat.m);
        _scene->draw();
        
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
}

- (void) initProgram {
    [self.mProgram build];
}

- (void) initTexture {
    [self.mTexture createTexture];
}

- (void) initObject3D {
    // load
    [self.mObject3D loadObj];
    
    // upload
    [self.mObject3D uploadDataToProgram:self.mProgram];
}

- (void) rendererOnDestroy:(EAGLContext*) context{
    
}

@end

@interface MD360RendererBuilder()
@property (nonatomic,readonly) MD360Director* director;
@property (nonatomic,readonly) MD360Texture* texture;
@end

@implementation MD360RendererBuilder

- (void) setDirector:(MD360Director*) director{
    _director = director;
}

- (void) setTexture:(MD360Texture*) texture{
    _texture = texture;
}

- (MD360Renderer*) build{
    MD360Renderer* renderer = [[MD360Renderer alloc]init];
    renderer.mDirector = self.director;
    renderer.mTexture = self.texture;
    return renderer;
}

@end
