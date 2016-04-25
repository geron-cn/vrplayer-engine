#include "VRPlayer.h"
#include "gameplay.h"

static gameplay::Texture *s_textrue = nullptr;

VRPlayer::VRPlayer()
: _isVREnabled(true)
{
    
}

VRPlayer::~VRPlayer()
{
    
}

bool VRPlayer::initalize()
{
    return true;
}

void VRPlayer::deInitalize()
{
    
}

bool VRPlayer::playVideo (const std::string& resourceName)
{
    return true;
}

/// Get the current playback time position in the video, in seconds
double VRPlayer::getCurrentTime()
{
    return 0.0;
}

/// Get the duration of the video in seconds
double VRPlayer::getDuration()
{
    return 0.0;
}

void VRPlayer::seek(double time)
{
    
}

void VRPlayer::play()
{
    
}
void VRPlayer::pause()
{
    
}
bool VRPlayer::isPaused()
{
    
}

bool VRPlayer::update()
{
    
}

void VRPlayer::close()
{
    
}

int VRPlayer::getVideoWidth()
{
    return 0;
}
int VRPlayer::getVideoHeight()
{
    return 0;
}

