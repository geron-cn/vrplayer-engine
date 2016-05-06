#include "StereoRender.h"
#include "SphereVideoSurround.h"
#include "VRPlayer.h"

StereoRender::StereoRender()
    : _eyeL(nullptr), _eyeR(nullptr), _head(nullptr), _videoSurround(nullptr)
{
}

StereoRender::~StereoRender()
{
    finalize();
}

void StereoRender::finalize()
{
    SAFE_RELEASE(_eyeL);
    SAFE_RELEASE(_eyeR);
    SAFE_RELEASE(_head);
    SAFE_DELETE(_videoSurround);
}

bool StereoRender::initialize(Node* parent, Scene* scene)
{
    finalize();
    
    _videoSurround = new SphereVideoSurround();
    _videoSurround->initialize(scene);
    
    float w = (float)Game::getInstance()->getWidth();
    float h = (float)Game::getInstance()->getHeight();
    float aspect = w / h;
    float focusLen = 300.f;
    float near = 0.1f;
    float far = 400.f;
    float fov = 85.f;
    _eyeL = Camera::createPerspective(fov, aspect, near, far);
    _eyeR = Camera::createPerspective(fov, aspect, near, far);
    
    _head = Node::create("stereorender_head");
    Node* eyeLNode = Node::create("stereorender_leye");
    Node* eyeRNode = Node::create("stereorender_reye");
    eyeLNode->setCamera(_eyeL);
    eyeRNode->setCamera(_eyeR);
    _head->addChild(eyeLNode);
    _head->addChild(eyeRNode);
    eyeLNode->release();
    eyeRNode->release();
    
    if (parent)
    {
        parent->addChild(_head);
    }
    else
        scene->addNode(_head);
    
    _head->setCamera(scene->getActiveCamera());
    
    /////////////////////////////////////////////////
    ////ref to http://paulbourke.net/stereographics/stereorender/
    //Toe-in method
    float eyeSep = 0.064f / 2.f;
    eyeLNode->setTranslationX(-eyeSep);
    eyeRNode->setTranslationX(eyeSep);
    
    auto pos = eyeLNode->getTranslation();
    Matrix rotation;
    Matrix::createLookAt(pos, pos + Vector3(0, 0, -focusLen), Vector3::unitY(), &rotation);
    eyeLNode->setRotation(rotation);
    
    pos = eyeRNode->getTranslation();
    Matrix::createLookAt(pos, pos + Vector3(0, 0, -focusLen),  Vector3::unitY(), &rotation);
    eyeRNode->setRotation(rotation);
    
    //correct method
    /*
    float eyeSep = 0.064f / 2.f;
    float eyeSepOnProjection = eyeSep * _nearPlane / _focusLen;
    float ymax = _nearPlane * tanf(PI * _fov * 0.5f / 180.f);
    float xmin, xmax;
    
    // translate xOffset
    GLKMatrix4 eyeLeft = GLKMatrix4Identity, eyeRight = GLKMatrix4Identity;
    GLKMatrix4 projectionL = _projectionMatrix;
    
    eyeLeft.m[ 12 ] = - eyeSep;
    eyeRight.m[ 12 ] = eyeSep;
    
    GLKMatrix4 eyeLeftView = GLKMatrix4Invert(eyeLeft, NULL);
    GLKMatrix4 eyeRightView = GLKMatrix4Invert(eyeRight, NULL);
    
    // for left eye
    xmin = - ymax * _aspect + eyeSepOnProjection;
    xmax = ymax * _aspect + eyeSepOnProjection;
    
    projectionL.m[ 0 ] = 2 * _nearPlane / ( xmax - xmin );
    projectionL.m[ 8 ] = ( xmax + xmin ) / ( xmax - xmin );
    
    
    // for right eye
    GLKMatrix4 projectionR = _projectionMatrix;
    
    xmin = - ymax * _aspect - eyeSepOnProjection;
    xmax = ymax * _aspect - eyeSepOnProjection;
    
    projectionR.m[ 0 ] = 2 * _nearPlane / ( xmax - xmin );
    projectionR.m[ 8 ] = ( xmax + xmin ) / ( xmax - xmin );
    
    glViewport(0, 0, _size.x / 2.f, _size.y);
    GLKMatrix4 mvp = GLKMatrix4Multiply(eyeLeftView, worldMat);
    mvp = GLKMatrix4Multiply(projectionL, mvp);
    [self renderEyeMVP:mvp numIndices:numIndices];
    
    glViewport(_size.x / 2.f, 0, _size.x / 2.f, _size.y);
    mvp = GLKMatrix4Multiply(eyeRightView, worldMat);
    mvp = GLKMatrix4Multiply(projectionR, mvp);
    [self renderEyeMVP:mvp numIndices:numIndices];
*/
    
    return true;
}

void StereoRender::updateHeadRotation()
{
    Matrix headview = Game::getInstance()->getHeadViewMatrix();
    headview.invert();
    _head->setRotation(headview);
}

bool StereoRender::setVideoURL(const char* url)
{
    if (_videoSurround)
        return _videoSurround->setVideoURL(url);
    return false;
}

void StereoRender::render(Scene* scene)
{
    if (Game::getInstance()->hasAccelerometer())
        updateHeadRotation();
    
    if (VRPlayer::getInstance()->isVREnabled())
    {
        auto oldCamera = scene->getActiveCamera();
        auto game = Game::getInstance();
        auto oldView = game->getViewport();
        
        Rectangle rect(0.f, 0.f, oldView.width / 2.f, oldView.height);
        game->setViewport(rect);
        scene->setActiveCamera(_eyeL);
        if (_videoSurround)
            _videoSurround->render(_eyeL);
        scene->visit(this, &StereoRender::drawScene);
        
        rect.x = oldView.width / 2.f;
        game->setViewport(rect);
        scene->setActiveCamera(_eyeR);
        if (_videoSurround)
            _videoSurround->render(_eyeR);
        scene->visit(this, &StereoRender::drawScene);
        
        game->setViewport(oldView);
        scene->setActiveCamera(oldCamera);
    }
    else{        
        if (_videoSurround)
            _videoSurround->render(scene->getActiveCamera());
        
        scene->visit(this, &StereoRender::drawScene);
    }
}

bool StereoRender::drawScene(Node* node)
{
    // If the node visited contains a drawable object, draw it
    Drawable* drawable = node->getDrawable();
    if (drawable)
        drawable->draw(false);
    
    return true;
}