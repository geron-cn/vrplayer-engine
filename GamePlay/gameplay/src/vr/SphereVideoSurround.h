#ifndef SphereVideoSurround_H_
#define SphereVideoSurround_H_

#include "gameplay.h"
//#include "VideoState.hpp"
//#include "videoplayer.hpp"

using namespace gameplay;

namespace vrliveff{
    class VideoState;
}
class AVFrame;
/**
 * Stereo Render.
 */
class SphereVideoSurround: public Ref
{
    friend class VRPlayer;
public:

    /**
     * Constructor.
     */
    SphereVideoSurround();

    virtual ~SphereVideoSurround();
    
    bool initialize(Scene* scene);
    
    void render(Camera* camera);
    
    bool setVideoURL(const std::string& url);

    double getVideoDuration() const;
    
    bool isPause() const;
    
    void pause(bool pause);
    
    double getCurrentPos() const;
    
    bool seek(double pos);

    void renderAVFrame(AVFrame *avframe);
//    Video::VideoPlayer* getVideoPlayer() const {return _player;}

protected:
    
    void finalize();

private:
    
    Node* _sphere;
    std::vector<uint8_t> _data;
    Texture*  _texture;
    int       _dstTextureW;
    int       _dstTextureH;
    int       _previewPts;
    vrliveff::VideoState* _videoState;
//    Video::VideoPlayer* _player;
    gameplay::Texture::Sampler* _sampler;
    
};

#endif
