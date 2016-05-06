#include "VRPlayer.h"
#include "VRGame.h"
#include "StereoRender.h"
#include "SphereVideoSurround.h"
#include "gameplay.h"

VRPlayer* VRPlayer::getInstance()
{
    static VRPlayer player;
    return &player;
}

VRPlayer::VRPlayer()
: _vrEnabled(true)
{
    
}

VRPlayer::~VRPlayer()
{
    
}

void* VRPlayer::initialize(int x, int y, int widith, int height)
{
    static VRGame game;
    
    return gameplay::Platform::initView();
}

void VRPlayer::finalize()
{
    stop();
    gameplay::Platform::finalize();
}

void VRPlayer::stop()
{
//    Platform::stopUpdating();
    _url = "";
    auto game = (VRGame*)Game::getInstance();
    if (game && game->getSteroRender())
    {
        game->getSteroRender()->setVideoURL(_url.c_str());
    }
}

bool VRPlayer::isPause() const
{
    auto game = (VRGame*)Game::getInstance();
    if (game && game->getSteroRender())
    {
        return game->getSteroRender()->getVideoSurround()->isPause();
    }
    return false;
}

bool VRPlayer::pause(bool p)
{
    auto game = (VRGame*)Game::getInstance();
    if (game && game->getSteroRender())
    {
        game->getSteroRender()->getVideoSurround()->pause(p);
    }
}

bool VRPlayer::seek(double pos)
{
    auto game = (VRGame*)Game::getInstance();
    if (game && game->getSteroRender())
    {
        return game->getSteroRender()->getVideoSurround()->seek(pos);
    }
    return true;
}

double VRPlayer::getCurrentPos() const
{
    auto game = (VRGame*)Game::getInstance();
    if (game && game->getSteroRender())
    {
        return game->getSteroRender()->getVideoSurround()->getCurrentPos();
    }
    return 0.0;
}

double VRPlayer::getDuration()
{
    auto game = (VRGame*)Game::getInstance();
    if (game && game->getSteroRender())
    {
        return game->getSteroRender()->getVideoSurround()->getVideoDuration();
    }
    return 0.0;
}

bool VRPlayer::play(const char* url)
{
    auto game = (VRGame*)Game::getInstance();
    Platform::startUpdating();
    if (game && game->getSteroRender())
    {
        bool ret = game->getSteroRender()->setVideoURL(url);
        _url = ret ? url : "";
        
        return ret;
    }
    _url = url;
    
    return true;
}
