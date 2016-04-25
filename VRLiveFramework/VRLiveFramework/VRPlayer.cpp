#include "VRPlayer.h"
#include "VRGame.h"
#include "StereoRender.h"
#include "SphereVideoSurround.h"
#include "audiodecoder.hpp"
#include "ALMovieAudioFactory.h"
#include "videoplayer.hpp"

VRPlayer::VRPlayer()
    : _isVREnabled(true)
    , _isRealtime(false)
    , _playerObj(nullptr)
{
    initalize();
}

VRPlayer::~VRPlayer()
{
    deInitalize();
}

#define GETVIDEOSTATE(STERORENDER, VIDEO) \
{\
    if(STERORENDER != nullptr){\
        auto render = STERORENDER->getSteroRender(); \
        if(render != nullptr){\
            auto rendersurround = render->getSphereVideoSurround();\
            if(render != nullptr)\
                VIDEO = rendersurround->getVideoPlayer();\
        }\
    }\
}

#define GETAUDIOSTATE(STERORENDER, AUDIO) \
{\
    Video::VideoPlayer* video = nullptr;\
    GETVIDEOSTATE(STERORENDER, video);\
    if(video != nullptr)\
        AUDIO = (ALMovieAudioDecoder*)video->getAudioFactory();\
}

bool VRPlayer::initalize()
{
    _playerObj = (VRGame*)gameplay::Game::getInstance();
    if( nullptr == _playerObj)
    {
        GP_WARN("init player root failed!");
        return false;
    }
    return true;
}

void VRPlayer::deInitalize()
{
}

bool VRPlayer::playVideo (const std::string& resourceName)
{
    Video::VideoPlayer* video = nullptr;
    GETVIDEOSTATE(_playerObj, video);
    if(video == nullptr)
    {
        GP_WARN("videostate is not initialized! ");
        return false;
    }
    video->playVideo(resourceName);
    return true;
}

/// Get the current playback time position in the video, in seconds
double VRPlayer::getCurrentTime()
{
    return 0.0;
}

/// Get the duration of the video in seconds
//double VRPlayer::getDuration()
//{
//    return 0.0;
//}
//
//void VRPlayer::seek(double time)
//{
//    
//}

void VRPlayer::play()
{
    Video::VideoPlayer* video = nullptr;
    GETVIDEOSTATE(_playerObj, video);
    if(video == nullptr)
    {
        GP_WARN("videostate is not initialized! ");
        return;
    }
    video->play();
}
void VRPlayer::pause()
{
    Video::VideoPlayer* video = nullptr;
    GETVIDEOSTATE(_playerObj, video);
    if(video == nullptr)
    {
        GP_WARN("videostate is not initialized! ");
        return;
    }
    video->pause();
}

bool VRPlayer::isPaused()
{
    Video::VideoPlayer* video = nullptr;
    GETVIDEOSTATE(_playerObj, video);
    if(video == nullptr)
    {
        GP_WARN("videostate is not initialized! ");
        return true;
    }
    return video->isPaused();
}


void VRPlayer::setVolume(float percent)
{
    ALMovieAudioDecoder* audio = nullptr;
    GETAUDIOSTATE(_playerObj, audio);
    if( nullptr == audio)
    {
        GP_WARN("audio is not initialized! ");
        return;
    }
    audio->setSourceGain(percent);
}

float VRPlayer::getVolume() const
{
    ALMovieAudioDecoder* audio = nullptr;
    GETAUDIOSTATE(_playerObj, audio);
    if( nullptr == audio)
    {
        GP_WARN("audio is not initialized! ");
        return 0.f;
    }
    return audio->getSourceGain();
}

//bool VRPlayer::update()
//{
//    return false;
//}

void VRPlayer::close()
{
    Video::VideoPlayer* video = nullptr;
    GETVIDEOSTATE(_playerObj, video);
    if(video == nullptr)
    {
        GP_WARN("videostate is not initialized! ");
    }
    video->close();
}

int VRPlayer::getVideoWidth() const
{
    Video::VideoPlayer* video = nullptr;
    GETVIDEOSTATE(_playerObj, video);
    if(video != nullptr)
        return video->getVideoWidth();
    return 0;
}

int VRPlayer::getVideoHeight() const
{
    Video::VideoPlayer* video = nullptr;
    GETVIDEOSTATE(_playerObj, video);
    if(video != nullptr)
        return video->getVideoHeight();
    return 0;
}

