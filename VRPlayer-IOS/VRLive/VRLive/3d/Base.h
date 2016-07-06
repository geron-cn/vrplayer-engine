//
// Created by tony
//
#ifndef BASE_H_
#define BASE_H_

namespace vrlive {
#ifdef __ANDROID__
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
    extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray;
    extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays;
    extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays;
    extern PFNGLISVERTEXARRAYOESPROC glIsVertexArray;
#define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
#define glClearDepth glClearDepthf
#define OPENGL_ES
#elif WIN32
#define WIN32_LEAN_AND_MEAN
#define GLEW_STATIC
#include <GL/glew.h>
#define GP_USE_VAO
#elif __linux__
#define GLEW_STATIC
#include <GL/glew.h>
#define GP_USE_VAO
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#define glBindVertexArray glBindVertexArrayOES
#define glDeleteVertexArrays glDeleteVertexArraysOES
#define glGenVertexArrays glGenVertexArraysOES
#define glIsVertexArray glIsVertexArrayOES
#define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
#define glClearDepth glClearDepthf
#define OPENGL_ES
#define GP_USE_VAO
#elif TARGET_OS_MAC
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#define glBindVertexArray glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glIsVertexArray glIsVertexArrayAPPLE
#define GP_USE_VAO
#else
#error "Unsupported Apple Device"
#endif
#endif

}

#endif
