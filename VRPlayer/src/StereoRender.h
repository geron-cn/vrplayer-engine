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
    
    bool isVREnabled() const { return _isVREnabled; }
    
    void setVREnabled(bool enable) { _isVREnabled = enable; }

protected:
    
    void updateHeadRotation();
    
    void finalize();

    bool drawScene(Node* node);

private:

    Camera* _eyeL; //left eye
    Camera* _eyeR; //right eye
    
    Node* _head;
    bool _isVREnabled;
    SphereVideoSurround* _videoSurround;
};

#endif
