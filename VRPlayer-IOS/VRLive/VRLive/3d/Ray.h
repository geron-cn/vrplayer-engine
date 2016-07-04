//
// Created by tony
//
#ifndef RAY_H_
#define RAY_H_
#include "Vector3.h"
#include "Matrix.h"
#include "BoundSphere.h"

namespace vrlive {
    class Ray
    {
    public:
        Ray(const Vector3& origin, const Vector3& dir);
        ~Ray();
        
        bool intersect(const BoundSphere* sphere, const Matrix* sphereMat, float* distance) const;
    public:
        Vector3 _origin;
        Vector3 _direction;
    };
}

#endif
