//
// Created by tony
//
#ifndef CAMERA_H_
#define CAMERA_H_
#include "Vector3.h"
#include "Node.h"
#include "Ref.h"

namespace vrlive {
    class Camera : public Ref
    {
    public:
        enum Type
        {
            PERSPECTIVE = 1,
            ORTHOGRAPHIC = 2
        };
        
        static Camera* createPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
        static Camera* createOrthographic(float zoomX, float zoomY, float aspectRatio, float nearPlane, float farPlane);
        
        const Matrix& getInverseViewMatrix() const;
        const Matrix& getViewMatrix() const;
        const Matrix& getProjectionMatrix() const;
        const Matrix& getViewProjectionMatrix() const;
        
        void setDirAndUp(const Vector3& dir, const Vector3& up);
        
        void setViewMatrix(float m[]);
        void setRotationMatrix(float m[]);
        
        void setLookingNode(Node* node) { _lookingNode = node; }
        
        Node* getLookingNode() const { return _lookingNode; }
        
        float getOrthWidth() const;
        float getOrthHeight() const;
        
    protected:
        /**
         * Constructor.
         */
        Camera(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
        
        /**
         * Constructor.
         */
        Camera(float zoomX, float zoomY, float aspectRatio, float nearPlane, float farPlane);
        
        /**
         * Destructor.
         */
        virtual ~Camera();
        
        Camera::Type _type;
        float _fieldOfView;
        float _zoom[2];
        float _aspectRatio;
        float _nearPlane;
        float _farPlane;
        mutable Matrix _view;
        mutable Matrix _projection;
        mutable Matrix _viewProjection;
        mutable Matrix _inverseView;
        mutable Matrix _inverseViewProjection;
        
        Node* _lookingNode; //weak ref
    };
}

#endif
