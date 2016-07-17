/*
 * IJKSDLGLView.m
 *
 * Copyright (c) 2013 Zhang Rui <bbcallen@gmail.com>
 *
 * based on https://github.com/kolyvan/kxmovie
 *
 * This file is part of ijkPlayer.
 *
 * ijkPlayer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * ijkPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with ijkPlayer; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#import "IJKSDLGLView.h"
#include "ijksdl/ijksdl_timer.h"
#include "ijksdl/ios/ijksdl_ios.h"
#include "ijksdl/ijksdl_gles2.h"
#include "ijksdl/ijksdl_mutex.h"
#import "IJKSDLHudViewController.h"

@interface IJKSDLGLView()
@property(atomic,strong) NSRecursiveLock *glActiveLock;
@property(atomic) BOOL glActivePaused;
@end

@implementation IJKSDLGLView {
    EAGLContext     *_context;
    GLuint          _framebuffer;
    GLuint          _renderbuffer;
    GLint           _backingWidth;
    GLint           _backingHeight;

    int             _frameCount;
    
    int64_t         _lastFrameTime;

    IJK_GLES2_Renderer *_renderer;
    int                 _rendererGravity;

    BOOL            _isRenderBufferInvalidated;

    int             _tryLockErrorCount;
    BOOL            _didSetupGL;
    BOOL            _didStopGL;
    NSMutableArray *_registeredNotifications;

    IJKSDLHudViewController *_hudViewController;
    
    //
    BOOL _hasNewFrame;
    SDL_VoutOverlay* _overlay;
    int _textureW;
    int _textureH;
    unsigned char* _textureData;
    NSLock *_textureLock;
}

static IJKSDLGLView* _instance = nil;

+ (Class) layerClass
{
	return [CAEAGLLayer class];
}

+ (id) instance
{
    return _instance;
}

- (id) initWithFrame:(CGRect)frame
{
    _hasNewFrame = NO;
    _overlay = nil;
    _textureH = 0;
    _textureW = 0;
    _textureData = NULL;
    
    _instance = self;
    _textureLock = [[NSLock alloc] init];
    return self;
}

- (BOOL)setupEAGLContext:(EAGLContext *)context
{
    return YES;
}

- (BOOL)setupGL
{
    return YES;
}

- (BOOL)setupGLOnce
{
    return YES;
}

- (void)dealloc
{
    IJK_GLES2_Renderer_reset(_renderer);
    IJK_GLES2_Renderer_freeP(&_renderer);
    
    _textureH = 0;
    _textureW = 0;
    _textureLock = nil;
    free(_textureData);
    
    _instance = nil;
}

- (void)setContentMode:(UIViewContentMode)contentMode
{
}

- (BOOL)setupRenderer: (SDL_VoutOverlay *) overlay
{
    if (overlay == nil)
        return _renderer != nil;

    if (!IJK_GLES2_Renderer_isValid(_renderer) ||
        !IJK_GLES2_Renderer_isFormat(_renderer, overlay->format)) {

        IJK_GLES2_Renderer_reset(_renderer);
        IJK_GLES2_Renderer_freeP(&_renderer);

        _renderer = IJK_GLES2_Renderer_create(overlay);
        if (!IJK_GLES2_Renderer_isValid(_renderer))
            return NO;

        if (!IJK_GLES2_Renderer_use(_renderer))
            return NO;

        IJK_GLES2_Renderer_setGravity(_renderer, _rendererGravity, _backingWidth, _backingHeight);
    }
    
    return YES;
}

- (void)invalidateRenderBuffer
{
}

- (BOOL) tryLockTextureData
{
    return [_textureLock tryLock];
}
- (void) unLockTextureData
{
    [_textureLock unlock];
}

- (int) getTextureWidth
{
    return _textureW;
}
- (int) getTextureHight
{
    return _textureH;
}
- (unsigned char*) popTextureData
{
    if (_hasNewFrame)
    {
        _hasNewFrame = NO;
        return _textureData;
    }
    return NULL;
}

- (void) updateTextureAndProgram: (float[])mvp posbuffer: (void*)pos texbuffer: (void*) tex
{
    if (_overlay == nil)
        return;
    
    //update if we have
    if (_hasNewFrame)
    {
        SDL_VoutLockYUVOverlay(_overlay);
        if (![self setupRenderer:_overlay]) {
            if (!_overlay && !_renderer) {
                NSLog(@"IJKSDLGLView: setupDisplay not ready\n");
            } else {
                NSLog(@"IJKSDLGLView: setupDisplay failed\n");
            }
            SDL_VoutUnlockYUVOverlay(_overlay);
            return;
        }
        
        if (!IJK_GLES2_Renderer_renderOverlay(_renderer, _overlay))
            ALOGE("[EGL] IJK_GLES2_render failed\n");
        
        SDL_VoutUnlockYUVOverlay(_overlay);
        _hasNewFrame = NO;
    }
    if (_renderer)
    {
        IJK_GLES2_Renderer_updateMVP(_renderer, mvp);
        
        IJK_GLES2_Renderer_updateVertexAndTex(_renderer, pos, tex);
    }
}

- (void)display: (SDL_VoutOverlay *) overlay
{
    if (![self setupGLOnce])
        return;

    [self displayInternal:overlay];
}

// NOTE: overlay could be NULl
- (void)displayInternal: (SDL_VoutOverlay *) overlay
{
    [_textureLock lock];
    
    if (_textureW != overlay->textureW || _textureH != overlay->textureH )
    {
        free(_textureData);
        _textureData = (unsigned char*)malloc(overlay->datasize);
        _textureW = overlay->textureW;
        _textureH = overlay->textureH;
    }
    memcpy(_textureData, overlay->data, overlay->datasize);
    
    _overlay = overlay;
    _hasNewFrame = YES;
    
    [_textureLock unlock];
}

- (void)registerApplicationObservers
{

}

- (void)unregisterApplicationObservers
{
}

#pragma mark snapshot

- (UIImage*)snapshot
{
    return nil;
}

- (void)setHudValue:(NSString *)value forKey:(NSString *)key
{
}

- (void)setShouldShowHudView:(BOOL)shouldShowHudView
{
}

- (BOOL)shouldShowHudView
{
    return NO;
}

@end
