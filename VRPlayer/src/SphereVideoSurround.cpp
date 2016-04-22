#include "SphereVideoSurround.h"
#include "ALMovieAudioFactory.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
    
    // From libavformat version 55.0.100 and onward the declaration of av_gettime() is
    // removed from libavformat/avformat.h and moved to libavutil/time.h
    // https://github.com/FFmpeg/FFmpeg/commit/06a83505992d5f49846c18507a6c3eb8a47c650e
#if AV_VERSION_INT(55, 0, 100) <= AV_VERSION_INT(LIBAVFORMAT_VERSION_MAJOR, \
LIBAVFORMAT_VERSION_MINOR, LIBAVFORMAT_VERSION_MICRO)
#include <libavutil/time.h>
#endif
    
#include <libavutil/mathematics.h>
    
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc  avcodec_alloc_frame
#endif
    
}



SphereVideoSurround::SphereVideoSurround(): _sphere(nullptr), _player(nullptr), _sampler(nullptr)
{
}

SphereVideoSurround::~SphereVideoSurround()
{
    finalize();
}

void SphereVideoSurround::finalize()
{
    SAFE_RELEASE(_sphere);
    SAFE_DELETE(_player);
    SAFE_RELEASE(_sampler);
}

bool SphereVideoSurround::initialize(Scene* scene)
{
    
    finalize();
    
    Bundle* bundle;
    bundle = Bundle::create("res/sphere.gpb");
    _sphere = bundle->loadNode("Sphere001");
    _sphere->setScale(300);
    _sphere->setRotation(Vector3(1, 0, 0), -MATH_PI / 2.f);
    SAFE_RELEASE(bundle);
    
    dynamic_cast<Model*>(_sphere->getDrawable())->setMaterial("res/myEarth.material#01___Default");
    
    scene->addNode(_sphere);
    
    _player = new Video::VideoPlayer();
    _player->setAudioFactory(new ALMovieAudioFactory());
//    setVideoURL("http://124.207.19.118:80/beijing-test/manifest.m3u8");
//    setVideoURL("http://qjdl.bravocloud.com.cn/android/android-12_05_15_comikazi_v01.mp4");
//    setVideoURL("2.ts");
//    setVideoURL("http://qjdldown.bravovcloud.com.cn/live/test.m3u8");
    setVideoURL(Game::getInstance()->getLiveURL());
//    setVideoURL("http://gotye-live-10022.ufile.ucloud.com.cn/08fc0f4a-5120-43dd-98f2-424660dd263c.mp4?k=52d00265544a4faf&t=1461064771");
//    setVideoURL("http://qjdlplay.bravovcloud.com.cn/live/test.m3u8");
    
    return true;
}

bool SphereVideoSurround::setVideoURL(const std::string& url)
{
    if (_player)
    {
        return _player->playVideo(url);
    }
    return false;
}


void SphereVideoSurround::render(Camera* camera)
{
    if (_player)
    {
        _player->update();
        
        if (_sampler == nullptr || _sampler->getTexture() != _player->getTexture())
        {
            if (_player->getTexture())
            {
                SAFE_RELEASE(_sampler);
                
                _sampler = Texture::Sampler::create(_player->getTexture());
                _sampler->setWrapMode(gameplay::Texture::CLAMP, gameplay::Texture::CLAMP);
                _sampler->setFilterMode(gameplay::Texture::LINEAR, gameplay::Texture::LINEAR);
                dynamic_cast<Model*>(_sphere->getDrawable())->getMaterial()->getTechnique()->getPassByIndex(0)->getParameter("u_diffuseTexture")->setSampler(_sampler);
            }
        }
    }
}
