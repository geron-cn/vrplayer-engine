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

    Scene* _scene;
    StereoRender* _stereoRender;
    gameplay::Form*             _messageForm;
};

#endif
