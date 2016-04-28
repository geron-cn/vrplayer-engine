#include "VRPlayer.h"
#include "VRGame.h"
#include "StereoRender.h"
#include "SphereVideoSurround.h"
#include "audiodecoder.hpp"
#include "ALMovieAudioFactory.h"
#include "videoplayer.hpp"
#include <string>


namespace vrlive
{
    const char* version()
    {
        return "0.0.1";
    }

    VRPlayer::VRPlayer()
        : _playerObj(nullptr)
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
            AUDIO = (Video::ALMovieAudioDecoder*)video->getAudioFactory();\
    }

    VRGame gameplayer;  // just for init Game::__instance
    bool VRPlayer::initalize()
    {
        auto game = gameplay::Game::getInstance();
        _playerObj = (VRGame*)game;
        if( nullptr == _playerObj)
        {
            GP_WARN("init player root failed!");
            return false;
        }
        return true;
    }

    static Platform* s_platform = nullptr;
    void VRPlayer::deInitalize()
    {
    //    close();
        _playerObj->exit();
        s_platform = nullptr;
    }

    bool VRPlayer::playVideo (const char* resourceName)
    {
        // filter live stream
        std::string suffixs[3] = {"rtsp://",".m3u8","rtmp://"};
        std::string url(resourceName);
        for (int i=0; i < 3; i++)
        {
            if(url.find(suffixs[i]) != -1)
            {
                GP_WARN("To get the support of this format, please contact the sdk provider! ");
                return false;
            }
        }
        
        s_platform = Platform::create(_playerObj);
        _playerObj->setLiveURL(resourceName);

        GP_ASSERT(platform);
        int result = s_platform->enterMessagePump();
        if(s_platform != nullptr)
            delete s_platform;
        return true;
    }

    ///// Get the current playback time position in the video, in seconds
    //double VRPlayer::getCurrentTime()
    //{
    //    Video::VideoPlayer* video = nullptr;
    //    GETVIDEOSTATE(_playerObj, video);
    //    if(video == nullptr)
    //    {
    //        GP_WARN("videostate is not initialized! ");
    //        return 0.0;
    //    }
    //    return video->getCurrentTime();
    //}
    //
    ///// Get the duration of the video in seconds
    ////double VRPlayer::getDuration()
    ////{
    ////    return 0.0;
    ////}
    ////
    ////void VRPlayer::seek(double time)
    ////{
    ////    
    ////}
    //
    //void VRPlayer::play()
    //{
    //    Video::VideoPlayer* video = nullptr;
    //    GETVIDEOSTATE(_playerObj, video);
    //    if(video == nullptr)
    //    {
    //        GP_WARN("videostate is not initialized! ");
    //        return;
    //    }
    //    video->play();
    //}
    //void VRPlayer::pause()
    //{
    //    Video::VideoPlayer* video = nullptr;
    //    GETVIDEOSTATE(_playerObj, video);
    //    if(video == nullptr)
    //    {
    //        GP_WARN("videostate is not initialized! ");
    //        return;
    //    }
    //    video->pause();
    //}
    //
    //bool VRPlayer::isPaused()
    //{
    //    Video::VideoPlayer* video = nullptr;
    //    GETVIDEOSTATE(_playerObj, video);
    //    if(video == nullptr)
    //    {
    //        GP_WARN("videostate is not initialized! ");
    //        return true;
    //    }
    //    return video->isPaused();
    //}
    //
    //
    //void VRPlayer::setVolume(float percent)
    //{
    //    Video::ALMovieAudioDecoder* audio = nullptr;
    //    GETAUDIOSTATE(_playerObj, audio);
    //    if( nullptr == audio)
    //    {
    //        GP_WARN("audio is not initialized! ");
    //        return;
    //    }
    //    audio->setSourceGain(percent);
    //}
    //
    //float VRPlayer::getVolume() const
    //{
    //    Video::ALMovieAudioDecoder* audio = nullptr;
    //    GETAUDIOSTATE(_playerObj, audio);
    //    if( nullptr == audio)
    //    {
    //        GP_WARN("audio is not initialized! ");
    //        return 0.f;
    //    }
    //    return audio->getSourceGain();
    //}
    //
    ////bool VRPlayer::update()
    ////{
    ////    return false;
    ////}
    //
    //void VRPlayer::close()
    //{
    //    Video::VideoPlayer* video = nullptr;
    //    GETVIDEOSTATE(_playerObj, video);
    //    if(video == nullptr)
    //    {
    //        GP_WARN("videostate is not initialized! ");
    //    }
    //    video->close();
    //    _playerObj->exit();
    //}
    //
    //int VRPlayer::getVideoWidth() const
    //{
    //    Video::VideoPlayer* video = nullptr;
    //    GETVIDEOSTATE(_playerObj, video);
    //    if(video != nullptr)
    //        return video->getVideoWidth();
    //    return 0;
    //}
    //
    //int VRPlayer::getVideoHeight() const
    //{
    //    Video::VideoPlayer* video = nullptr;
    //    GETVIDEOSTATE(_playerObj, video);
    //    if(video != nullptr)
    //        return video->getVideoHeight();
    //    return 0;
    //}
}
