#include "SphereVideoSurround.h"
//#include "ALMovieAudioFactory.h"
#include "SpriteBatch.h"
#include "ScreenDisplayer.h"
#include "VRPlayer.h"

#define IGNORE_SDLMAIN
#ifdef __cplusplus
extern "C" {
#endif
    
#include "VideoState.cpp"
    
#ifdef __cplusplus
}
#endif

extern "C"
{    
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

static int s_maxTextureSize = 0;

SphereVideoSurround::SphereVideoSurround()
    : _sphere(nullptr)
  //  , _player(nullptr)
    , _sampler(nullptr)
    , _videoState(nullptr)
    , _texture(nullptr)
{
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &s_maxTextureSize);
}

SphereVideoSurround::~SphereVideoSurround()
{
    finalize();
}

void SphereVideoSurround::finalize()
{
    SAFE_RELEASE(_sphere);
//    SAFE_DELETE(_player);
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
    
    setVideoURL(VRPlayer::getInstance()->_url);
    
//    _player = new Video::VideoPlayer();
//    _player->setAudioFactory(new Video::ALMovieAudioFactory());
//    setVideoURL("http://124.207.19.118:80/beijing-test/manifest.m3u8");
//    setVideoURL("http://qjdl.bravocloud.com.cn/android/android-12_05_15_comikazi_v01.mp4");
//    setVideoURL("2.ts");
//    setVideoURL("http://qjdldown.bravovcloud.com.cn/live/test.m3u8");
//      setVideoURL(Game::getInstance()->getLiveURL());
//    setVideoURL("http://gotye-live-10022.ufile.ucloud.com.cn/08fc0f4a-5120-43dd-98f2-424660dd263c.mp4?k=52d00265544a4faf&t=1461064771");
//    setVideoURL("http://qjdlplay.bravovcloud.com.cn/live/test.m3u8");
    
    return true;
}


double remaining_time =  REFRESH_RATE;
bool SphereVideoSurround::setVideoURL(const std::string& url)
{
    if(_videoState != nullptr)
    {
        vrliveff::do_exit(_videoState);
        _videoState = nullptr;
        vrliveff::customRenderFrame = nullptr;
    }
    if (url.empty())
        return false;
    
    _videoState = vrliveff::init_videostate(url.c_str());
    
    _dstTextureW = 0;
    _dstTextureH = 0;
    //if (_player)
//    {
//        
//        return _player->playVideo(url);
//    }
    return _videoState != nullptr;
}

double SphereVideoSurround::getVideoDuration() const
{
    if (_videoState && _videoState->ic)
    {
        if (_videoState->ic->duration < 0)
            return 0.0;
        return ((double)_videoState->ic->duration) /  1000000LL;
    }
    return 0.0;
}

bool SphereVideoSurround::isPause() const
{
    if (_videoState)
    {
        return _videoState->paused;
    }
    return false;
}

void SphereVideoSurround::pause(bool pause)
{
    if (_videoState)
    {
        if (pause == isPause())
            return;
        toggle_pause(_videoState);
    }
}

bool SphereVideoSurround::seek(double pos)
{
    if (_videoState)
    {
        if (_videoState->ic->start_time != AV_NOPTS_VALUE)
            pos += _videoState->ic->start_time;
        stream_seek(_videoState, pos, 0, 0);
        return true;
    }
    return false;
}

double SphereVideoSurround::getCurrentPos() const
{
    if (_videoState)
    {
        double pos = get_master_clock(_videoState);
        if (isnan(pos))
            pos = (double)_videoState->seek_pos / AV_TIME_BASE;
        return pos;
    }
    return 0.0;
}

void SphereVideoSurround::render(Camera* camera)
{
    remaining_time = REFRESH_RATE;
    if (_videoState && _videoState->show_mode != vrliveff::VideoState::ShowMode::SHOW_MODE_NONE
        && (!_videoState->paused || _videoState->force_refresh))
    {
        vrliveff::video_refresh(_videoState, &remaining_time);

        //if (_player)
        //{
//        _player->update();
//
        if(vrliveff::customRenderFrame != nullptr)
        {
            auto src_frame = vrliveff::customRenderFrame->frame;
            if(src_frame->data[0] == nullptr)
                return;
            uint8_t *dst_data[4];
            int dst_linesize[4] = {0, 0, 0, 0};
            if (_dstTextureH == 0 || _dstTextureW == 0)
            {
                int codecWidth = src_frame->width;
                int codecHeight = src_frame->height;
                if (s_maxTextureSize < codecWidth && codecWidth > codecHeight)
                {
                    _dstTextureW = s_maxTextureSize;
                    _dstTextureH = (int)(((float)s_maxTextureSize / codecWidth) * codecHeight);
                }
                else if (s_maxTextureSize < codecHeight && codecHeight > codecWidth)
                {
                    _dstTextureH = s_maxTextureSize;
                    _dstTextureW = (int)(((float)s_maxTextureSize / codecHeight) * codecWidth);
                }
                else
                {
                    _dstTextureW = codecWidth;
                    _dstTextureH = codecHeight;
                }
            }
            
            _videoState->img_convert_ctx = sws_getCachedContext(_videoState->img_convert_ctx,
                                           src_frame->width, src_frame->height, (AVPixelFormat)src_frame->format,
                                           _dstTextureW, _dstTextureH,
                                           AV_PIX_FMT_RGB24, vrliveff::sws_flags, NULL, NULL, NULL);
            if (_videoState->img_convert_ctx == NULL) {
                GP_WARN( "Cannot initialize the conversion context\n");
                return;
            }
            static std::vector<uint8_t> data;
            data.resize(_dstTextureW * _dstTextureH * 3);
            uint8_t *dst = &data[0];
            dst_linesize[0] = _dstTextureW * 3;
            
            sws_scale(_videoState->img_convert_ctx, src_frame->data, src_frame->linesize,
                      0, src_frame->height, &dst, dst_linesize);

            
            if (_texture == nullptr)
            {
                _texture = gameplay::Texture::create(gameplay::Texture::Format::RGB,
                                                     _dstTextureW, _dstTextureH, (unsigned char*)dst);
            }
            else
            {
                _texture->setData((unsigned char*)dst);
            }
            av_frame_unref(vrliveff::customRenderFrame->frame);
        }
        if (_sampler == nullptr || _sampler->getTexture() != _texture)
        {
            if (_texture)
            {
                SAFE_RELEASE(_sampler);
                
                _sampler = Texture::Sampler::create(_texture);
                _sampler->setWrapMode(gameplay::Texture::CLAMP, gameplay::Texture::CLAMP);
                _sampler->setFilterMode(gameplay::Texture::LINEAR, gameplay::Texture::LINEAR);
                dynamic_cast<Model*>(_sphere->getDrawable())->getMaterial()->getTechnique()->getPassByIndex(0)->getParameter("u_diffuseTexture")->setSampler(_sampler);
            }
        }
    }
}

