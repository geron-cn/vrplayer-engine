#include <math.h>
#include "Ray.h"


namespace vrlive {
    Ray::Ray(const Vector3& origin, const Vector3& dir)
    : _origin(origin)
    , _direction(dir)
    {
        
    }
    Ray::~Ray()
    {
        
    }
    
    bool Ray::intersect(const BoundSphere* sphere, const Matrix* sphereMat, float* distance) const
    {
        auto center = sphere->getCenter();
        if (sphereMat)
        {
            sphereMat->transformPoint(&center);
        }
        auto off = (center - sphere->getCenter());
        float offlen = off.length();
        float cs = Vector3::dot(off, _direction) / (offlen * _direction.length());
        if (cs > 1.f)
            cs = 1.f;
        
        float len = sqrtf(1 - cs * cs) * offlen;
        if (distance)
            *distance = len;
        
        return (len < sphere->getRadius());
    }
}
