//
//  MDAbsObject3D.m
//  MD360Player4IOS
//
//  Created by ashqal on 16/4/6.
//  Copyright © 2016年 ashqal. All rights reserved.
//

#include <vector>
#import "MDAbsObject3D.h"
#import "GLUtil.h"
#import "MDVRLibrary.h"
#import "3d/VertexBuffer.h"

static int sPositionDataSize = 3;
@interface MDAbsObject3D(){
    int positionHandle;
    int textureCoordinateHandle;
    vrlive::VertexBuffer* vertexbuff;
}

@end
@implementation MDAbsObject3D

- (instancetype)init
{
    self = [super init];
    if (self) {
        positionHandle = -1;
        textureCoordinateHandle = -1;
        vertexbuff = NULL;
    }
    return self;
}

- (void) destroy {
    if (mVertexBuffer != NULL)  free(mVertexBuffer);
    if (mTextureBuffer != NULL)  free(mTextureBuffer);
    if (mIndicesBuffer != NULL) free(mIndicesBuffer);
    
    if (positionHandle != -1){
        glDisableVertexAttribArray(positionHandle);
        positionHandle = -1;
    }
    
    if (textureCoordinateHandle != -1){
        glDisableVertexAttribArray(textureCoordinateHandle);
        textureCoordinateHandle = -1;
    }
    if (vertexbuff != NULL)
    {
        vertexbuff->release();
        vertexbuff = NULL;
    }
}

- (void)setVertexBuffer:(float*)buffer size:(int)size{
    int size_t = sizeof(float)*size;
    mVertexBuffer = (float*)malloc(size_t);
    memcpy(mVertexBuffer, buffer, size_t);
}

- (void)setIndicesBuffer:(short *)buffer size:(int)size{
    int size_t = sizeof(short)*size;
    mIndicesBuffer = (short*)malloc(size_t);
    assert(mIndicesBuffer);
    memcpy(mIndicesBuffer, buffer, size_t);
}

- (short*) getIndices{
    return mIndicesBuffer;
}

- (float*) getVertexBuffer
{
    return mVertexBuffer;
}

- (float*) getTextureBuffer
{
    return mTextureBuffer;
}

- (void)setTextureBuffer:(float*)buffer size:(int)size{
    int size_t = sizeof(float)*size;
    mTextureBuffer = (float*)malloc(size_t);
    memcpy(mTextureBuffer, buffer, size_t);
}

- (void)setNumIndices:(int)value{
    _mNumIndices = value;
}

- (void)setVertexBuffer:(vrlive::VertexBuffer*) vertbuf
{
    vertexbuff = vertbuf;
}

- (void)loadObj{
    NSString* obj3dPath = [self obtainObjPath];
    if (obj3dPath == nil) {
        NSLog(@"obj3dPath can't be null");
        return;
    }
    [GLUtil loadObject3DWithPath:obj3dPath output:self];
    //[GLUtil loadObject3DMock:self];
}


- (void)uploadDataToProgram:(MD360Program*)program{
    positionHandle = program.mPositionHandle;
    glEnableVertexAttribArray(positionHandle);
    glVertexAttribPointer(positionHandle, sPositionDataSize, GL_FLOAT, 0, 0, mVertexBuffer);
    // glDisableVertexAttribArray(positionHandle);
    
    textureCoordinateHandle = program.mTextureCoordinateHandle;
    glEnableVertexAttribArray(textureCoordinateHandle);
    glVertexAttribPointer(textureCoordinateHandle, 2, GL_FLOAT, 0, 0, mTextureBuffer);
    // glDisableVertexAttribArray(textureCoordinateHandle);
//    positionHandle = program.mPositionHandle;
//    textureCoordinateHandle = program.mTextureCoordinateHandle;
//    
//    vertexbuff->bind(positionHandle, textureCoordinateHandle);
}


- (void)onDraw{
    
    if ([self getIndices] != 0) {
        GLsizei count = self.mNumIndices;
        const GLvoid* indices = [self getIndices];
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, indices);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, self.mNumIndices);
    }
    // Draw
    [GLUtil glCheck:@"glDrawArrays"];
}

-(NSString *)description{

    NSMutableString* vertexSB = [[NSMutableString alloc]init];
    for (int i = 0; i < self.mNumIndices * 3; i++) {
        [vertexSB appendFormat:@"%f ",mVertexBuffer[i]];
    }
    NSMutableString* textureSB = [[NSMutableString alloc]init];
    for (int i = 0; i < self.mNumIndices * 2; i++) {
        [textureSB appendFormat:@"%f ",mTextureBuffer[i]];
    }
    NSString* result = [NSString stringWithFormat:@"loadObject3D complete:\n %@\n\n %@\n\n %d\n\n",vertexSB,textureSB,self.mNumIndices];
    return result;
}
@end

#pragma mark MDSphere3D48
@implementation MDSphere3D48

-(NSString*) obtainObjPath{
    //return [MDVR_RAW pathForResource:@"sphere48" ofType:@"obj"];
    return nil;
}

@end

#pragma mark MDSphere3D
@implementation MDSphere3D

-(NSString*) obtainObjPath{
    return nil;
}

- (void)loadObj{
    generateSphere([MDVRLibrary getSphereRadius],80,self);
}

#define ES_PI  (3.14159265f)

#pragma mark generate sphere
int generateSphere (float radius, int numSlices, MDAbsObject3D* object3D) {
    int i;
    int j;
    int numParallels = numSlices / 2;
    int numVertices = ( numParallels + 1 ) * ( numSlices + 1 );
    int numIndices = numParallels * numSlices * 6;
    float angleStep = (2.0f * ES_PI) / ((float) numSlices);
    
    float* vertices = (float*)malloc ( sizeof(float) * 3 * numVertices );
    float* texCoords = (float*)malloc ( sizeof(float) * 2 * numVertices );
     short* indices = ( short*)malloc ( sizeof(unsigned short) * numIndices );
    
    
    for ( i = 0; i < numParallels + 1; i++ ) {
        for ( j = 0; j < numSlices + 1; j++ ) {
            int vertex = ( i * (numSlices + 1) + j ) * 3;
            
            if ( vertices ) {
                vertices[vertex + 0] = - radius * sinf ( angleStep * (float)i ) * sinf ( angleStep * (float)j );
                vertices[vertex + 1] = - radius * cosf ( angleStep * (float)i );
                vertices[vertex + 2] = radius * sinf ( angleStep * (float)i ) * cosf ( angleStep * (float)j );
            }
            
            if (texCoords) {
                int texIndex = ( i * (numSlices + 1) + j ) * 2;
                texCoords[texIndex + 0] = (float) j / (float) numSlices;
                texCoords[texIndex + 1] = 1.0f - ((float) i / (float) (numParallels));
            }
        }
    }
    
    // Generate the indices
    if ( indices != NULL ) {
         short* indexBuf = indices;
        for ( i = 0; i < numParallels ; i++ ) {
            for ( j = 0; j < numSlices; j++ ) {
                *indexBuf++ = ( short)(i * ( numSlices + 1 ) + j); // a
                *indexBuf++ = ( short)(( i + 1 ) * ( numSlices + 1 ) + j); // b
                *indexBuf++ = ( short)(( i + 1 ) * ( numSlices + 1 ) + ( j + 1 )); // c
                
                
                *indexBuf++ = ( short)(i * ( numSlices + 1 ) + j); // a
                *indexBuf++ = ( short)(( i + 1 ) * ( numSlices + 1 ) + ( j + 1 )); // c
                *indexBuf++ = ( short)(i * ( numSlices + 1 ) + ( j + 1 )); // d
                
            }
        }
        
    }
    
//    std::vector<float> pos(vertices, vertices+numVertices);
//    std::vector<float> tex(texCoords, texCoords+numVertices*2);
//    std::vector<unsigned short> idx(indices, indices+numIndices);
//    std::vector<float> normal;
//    vrlive::VertexBuffer* vertbuf = vrlive::VertexBuffer::create(pos, normal, tex, idx);
//    [object3D setVertexBuffer: vertbuf];
    
    
    [object3D setIndicesBuffer:indices size:numIndices]; //object3D.setIndicesBuffer(indexBuffer);
    [object3D setTextureBuffer:texCoords size:2 * numVertices]; //object3D.setTexCoordinateBuffer(texBuffer);
    [object3D setVertexBuffer:vertices size: 3 * numVertices]; //object3D.setVerticesBuffer(vertexBuffer);
    [object3D setNumIndices:numIndices];
    
    
    free(indices);
    free(texCoords);
    free(vertices);
    
    return numIndices;
}
@end

#pragma mark MDDome3D
@implementation MDDome3D

-(NSString*) obtainObjPath{
    return [MDVR_RAW pathForResource:@"dome" ofType:@"obj"];
}

@end
