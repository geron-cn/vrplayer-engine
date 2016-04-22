#include "videoplayer.hpp"

#include "audiofactory.hpp"
#include "videostate.hpp"

namespace Video
{

VideoPlayer::VideoPlayer()
    : mState(NULL)
{

}

VideoPlayer::~VideoPlayer()
{
    if(mState)
        close();
}

void VideoPlayer::setAudioFactory(MovieAudioFactory *factory)
{
    mAudioFactory.reset(factory);
}

bool VideoPlayer::playVideo(const std::string &resourceName)
{
    if(mState)
        close();

//    try {
        mState = new VideoState;
        mState->setAudioFactory(mAudioFactory.get());
        if (!mState->init(resourceName))
        {
            GP_WARN("Failed to play video");
            return false;
        }

        // wait until we have the first picture
        while (mState->video_st && mState->mTexture)
        {
            if (!mState->update())
                break;
        }
//    }
//    catch(std::exception& e) {
//        std::string message = std::string("Failed to play video") + std::string(e.what());
//        GP_WARN(message.c_str());
//        close();
//        return false;
//    }
    return true;
}

bool VideoPlayer::update ()
{
    if(mState)
        return mState->update();
    return false;
}

std::string VideoPlayer::getTextureName()
{
    std::string name;
    if (mState && !mState->mTexture)
        name = mState->mTexture->getHandle();
    return name;
}

int VideoPlayer::getVideoWidth()
{
    int width=0;
    if (mState && !mState->mTexture)
        width = mState->mTexture->getWidth();
    return width;
}

int VideoPlayer::getVideoHeight()
{
    int height=0;
    if (mState && !mState->mTexture)
        height = mState->mTexture->getHeight();
    return height;
}

void VideoPlayer::close()
{
    if(mState)
    {
        mState->deinit();

        delete mState;
        mState = NULL;
    }
}

bool VideoPlayer::hasAudioStream()
{
    return mState && mState->audio_st != NULL;
}

void VideoPlayer::play()
{
    if (mState)
        mState->setPaused(false);
}

void VideoPlayer::pause()
{
    if (mState)
        mState->setPaused(true);
}

bool VideoPlayer::isPaused()
{
    if (mState)
        return mState->mPaused;
    return true;
}

double VideoPlayer::getCurrentTime()
{
    if (mState)
        return mState->get_master_clock();
    return 0.0;
}

void VideoPlayer::seek(double time)
{
    if (mState)
        mState->seekTo(time);
}

double VideoPlayer::getDuration()
{
    if (mState)
        return mState->getDuration();
    return 0.0;
}
    
gameplay::Texture* VideoPlayer::getTexture()
{
    if (mState)
    {
        return mState->mTexture;
    }
    return nullptr;
}

}
