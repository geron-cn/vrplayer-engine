//
//  MD360Program.m
//  MD360Player4IOS
//
//  Created by ashqal on 16/4/6.
//  Copyright © 2016年 ashqal. All rights reserved.
//

#import "MD360Program.h"
#import "GLUtil.h"
@interface MD360Program(){
    GLuint vertexShaderHandle,fragmentShaderHandle;
}
@end

@implementation MD360Program
- (void) build {
    NSString* vertexShader = [self getVertexShader];
    NSString* fragmentShader = [self getFragmentShader];
    // NSLog(@"%@ %@",vertexShader,fragmentShader);
    //GLuint vertexShaderHandle,fragmentShaderHandle;
    
    if (![GLUtil compileShader:&vertexShaderHandle type:GL_VERTEX_SHADER source:vertexShader])
        NSLog(@"Failed to compile vertex shader");
    
    if (![GLUtil compileShader:&fragmentShaderHandle type:GL_FRAGMENT_SHADER source:fragmentShader])
        NSLog(@"Failed to compile fragment shader");
    
    NSArray* attrs = [[NSArray alloc] initWithObjects:@"a_Position", @"a_TexCoordinate", nil];
    _mProgramHandle = [GLUtil createAndLinkProgramWith:vertexShaderHandle fsHandle:fragmentShaderHandle attrs:attrs];

    _mMVMatrixHandle = glGetUniformLocation(self.mProgramHandle, "u_MVMatrix");
    _mMVPMatrixHandle = glGetUniformLocation(self.mProgramHandle, "u_MVPMatrix");
    _mTextureUniformHandle = glGetUniformLocation(self.mProgramHandle, "u_Texture");
    
    _mPositionHandle = glGetAttribLocation(self.mProgramHandle, "a_Position");
    _mTextureCoordinateHandle = glGetAttribLocation(self.mProgramHandle, "a_TexCoordinate");
}

- (void) destroy {
    if (vertexShaderHandle) glDeleteShader(vertexShaderHandle);
    if (fragmentShaderHandle) glDeleteShader(fragmentShaderHandle);
    if (self.mProgramHandle) glDeleteProgram(self.mProgramHandle);
    vertexShaderHandle = fragmentShaderHandle = _mProgramHandle = 0;
}


- (void) use {
    if(self.mProgramHandle) glUseProgram(self.mProgramHandle);
}

- (void)unuse
{
    glUseProgram(0);
}

- (NSString*) getVertexShader {
    
    NSString* text = @"uniform mat4 u_MVPMatrix;\n\
    uniform mat4 u_MVMatrix;		// A constant representing the combined model/view matrix.\n\
    attribute vec4 a_Position;		// Per-vertex position information we will pass in.\n\
    attribute vec3 a_Normal;		// Per-vertex normal information we will pass in.\n\
    attribute vec2 a_TexCoordinate; // Per-vertex texture coordinate information we will pass in.\n\
    varying vec2 v_TexCoordinate;   // This will be passed into the fragment shader.\n\
    void main()\n \
    { \n \
        v_TexCoordinate = a_TexCoordinate;\n \
        gl_Position = u_MVPMatrix * a_Position;\n \
    }";
    return text;
}

- (NSString*) getFragmentShader {
    NSString* text = @"precision mediump float;\
    uniform sampler2D u_Texture;\
    varying vec2 v_TexCoordinate;\
    void main()\
    {\
        gl_FragColor =  texture2D(u_Texture, v_TexCoordinate);\
    }";
    return text;
}

@end
