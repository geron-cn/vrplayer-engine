#ifndef SphereVideoSurround_H_
#define SphereVideoSurround_H_

#include "gameplay.h"
#include "videoplayer.hpp"

using namespace gameplay;

/**
 * Stereo Render.
 */
class SphereVideoSurround: public Ref
{
public:

    /**
     * Constructor.
     */
    SphereVideoSurround();

    virtual ~SphereVideoSurround();
    
    bool initialize(Scene* scene);
    
    void render(Camera* camera);
    
    bool setVideoURL(const std::string& url);


    Video::VideoPlayer* getVideoPlayer() const {return _player;}

protected:
    
    void finalize();

private:
    
    Node* _sphere;
    
    Video::VideoPlayer* _player;
    gameplay::Texture::Sampler* _sampler;
    
};

#endif
