#ifndef StereoRender_H_
#define StereoRender_H_

#include "gameplay.h"

using namespace gameplay;

class SphereVideoSurround;

/**
 * Stereo Render.
 */
class StereoRender: public Ref
{
public:

    /**
     * Constructor.
     */
    StereoRender();

    virtual ~StereoRender();
    
    bool initialize(Node* parent, Scene* scene);
    
    void render(Scene* scene);
    
    bool setVideoURL(const char* url);
    
    SphereVideoSurround* getVideoSurround() const { return _videoSurround; }
    
protected:
    
    void updateHeadRotation();
    
    void finalize();

    bool drawScene(Node* node);
    
    void setEyeFov(float fov);

private:

    Camera* _eyeL; //left eye
    Camera* _eyeR; //right eye
    float _eyeFov;
    
    Node* _head;
    SphereVideoSurround* _videoSurround;
};

#endif
