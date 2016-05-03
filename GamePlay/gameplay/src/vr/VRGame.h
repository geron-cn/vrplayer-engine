#ifndef VRGame_H_
#define VRGame_H_

#include "gameplay.h"


using namespace gameplay;

class StereoRender;
/**
 * Main game class.
 */
class VRGame: public Game
{
public:

    /**
     * Constructor.
     */
    VRGame();

    /**
     * @see Game::keyEvent
     */
	void keyEvent(Keyboard::KeyEvent evt, int key);
	
    /**
     * @see Game::touchEvent
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
    
    StereoRender* getSteroRender() const { return _stereoRender;}
    
protected:

    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::finalize
     */
    void finalize();

    /**
     * @see Game::update
     */
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);

private:

    /**
     * Draws the scene each frame.
     */
    bool drawScene(Node* node);
    
    Form* _activeForm;
    static const char LOGO_PNG_DATA[524289]; // 256*256*4*2 + 1

    Scene* _scene;
    StereoRender* _stereoRender;
    gameplay::Form*             _messageForm;
};

#endif
