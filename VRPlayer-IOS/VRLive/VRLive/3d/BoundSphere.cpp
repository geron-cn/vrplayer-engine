#include "BoundSphere.h"

namespace vrlive {
    
    BoundSphere* BoundSphere::create(const Vector3& center, float radius)
    {
        auto sphere = new BoundSphere(center, radius);
        
        return sphere;
    }
    
    BoundSphere::BoundSphere(const Vector3& center, float radius)
    : _center(center)
    , _radius(radius)
    {
        
    }
    
    BoundSphere::~BoundSphere()
    {
        
    }
}

