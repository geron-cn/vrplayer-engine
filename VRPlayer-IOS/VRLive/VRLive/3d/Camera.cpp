#include "Camera.h"
#include "Matrix.h"

namespace vrlive {
    
Camera* Camera::createPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
    return new Camera(fieldOfView, aspectRatio, nearPlane, farPlane);
}
Camera* Camera::createOrthographic(float zoomX, float zoomY, float aspectRatio, float nearPlane, float farPlane)
{
    return new Camera(zoomX, zoomY, aspectRatio, nearPlane, farPlane);
}

const Matrix& Camera::getInverseViewMatrix() const
{
    return _inverseView;
}
    
const Matrix& Camera::getViewMatrix() const
{
    return _view;
}
const Matrix& Camera::getProjectionMatrix() const
{
    return _projection;
}
const Matrix& Camera::getViewProjectionMatrix() const
{
    Matrix::multiply(getProjectionMatrix(), getViewMatrix(), &_viewProjection);

    return _viewProjection;
}

void Camera::setDirAndUp(const Vector3& dir, const Vector3& up)
{
    Matrix::createLookAt(Vector3::zero(), dir, up, &_inverseView);
    _inverseView.invert(&_view);
}
    
void Camera::setViewMatrix(float m[])
{
    _view.set(m);
    _view.invert(&_inverseView);
}
    
void Camera::setRotationMatrix(float m[])
{
    
        
}
    
Camera::~Camera()
{
    
}

Camera::Camera(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
: _type(PERSPECTIVE), _fieldOfView(fieldOfView), _aspectRatio(aspectRatio), _nearPlane(nearPlane), _farPlane(farPlane)
{
    Matrix::createPerspective(fieldOfView, aspectRatio, nearPlane, farPlane, &_projection);
}
    
Camera::Camera(float zoomX, float zoomY, float aspectRatio, float nearPlane, float farPlane)
    : _type(ORTHOGRAPHIC), _aspectRatio(aspectRatio), _nearPlane(nearPlane), _farPlane(farPlane)
{
    // Orthographic camera.
    _zoom[0] = zoomX;
    _zoom[1] = zoomY;
    Matrix::createOrthographic(zoomX, zoomY, _nearPlane, _farPlane, &_projection);
}
}
