#ifndef SphereVideoSurround_H_
#define SphereVideoSurround_H_

#include "gameplay.h"
//#include "VideoState.hpp"
//#include "videoplayer.hpp"

using namespace gameplay;

namespace vrliveff{
    class VideoState;
}
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


//    Video::VideoPlayer* getVideoPlayer() const {return _player;}

protected:
    
    void finalize();

private:
    
    Node* _sphere;
    
    Texture*  _texture;
    int       _dstTextureW;
    int       _dstTextureH;
    vrliveff::VideoState* _videoState;
//    Video::VideoPlayer* _player;
    gameplay::Texture::Sampler* _sampler;
    
};

#endif
