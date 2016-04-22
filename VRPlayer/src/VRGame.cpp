#include "VRGame.h"
#include "StereoRender.h"

// Declare our game instance
VRGame game;

VRGame::VRGame()
    : _scene(NULL), _stereoRender(nullptr)
{
}

void VRGame::initialize()
{
    // Load game scene from file
    _scene = Scene::create();//
    
    float near = 0.1f;
    float far = 400.f;
    float fov = 85.f;
    auto camera = Camera::createPerspective(fov, Game::getInstance()->getAspectRatio(), near, far);
    _scene->setActiveCamera(camera);
    
    _stereoRender = new StereoRender();
    _stereoRender->initialize(nullptr, _scene);
    
    setDesiredFPS(40);
}

void VRGame::finalize()
{
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_stereoRender);
}

void VRGame::update(float elapsedTime)
{

}

void VRGame::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);
    if (_stereoRender)
        _stereoRender->render(_scene);
}

bool VRGame::drawScene(Node* node)
{
    // If the node visited contains a drawable object, draw it
    Drawable* drawable = node->getDrawable(); 
    if (drawable)
        drawable->draw(false);

    return true;
}

void VRGame::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_ESCAPE:
            exit();
            break;
        }
    }
}

void VRGame::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        break;
    case Touch::TOUCH_RELEASE:
        break;
    case Touch::TOUCH_MOVE:
        break;
    };
}
