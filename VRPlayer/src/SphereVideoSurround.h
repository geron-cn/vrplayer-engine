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

    static const char LOGO_PNG_DATA[524289]; // 256*256*4*2 + 1
    Texture*  _logoTexture;
    void drawSplash(void* param);
    
    Node* _sphere;
    
    Video::VideoPlayer* _player;
    gameplay::Texture::Sampler* _sampler;
    
};

#endif
