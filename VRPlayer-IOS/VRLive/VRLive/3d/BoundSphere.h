//
// Created by tony
//
#ifndef BoundSPHERE_H_
#define BoundSPHERE_H_
#include "Vector3.h"
#include "Ref.h"
#include <string>

namespace vrlive {
    
    
    class BoundSphere : public Ref
    {
    public:
        static BoundSphere* create(const Vector3& center, float radius);
        
        void setCenter(const Vector3& center) { _center = center; }
        
        void setRadius(float radius) { _radius = radius; }
        
        const Vector3& getCenter() const { return _center; }
        
        float getRadius() const { return _radius; }
        
    protected:
        
        BoundSphere(const Vector3& center, float radius);
        
        virtual ~BoundSphere();
        
    protected:
        
        Vector3 _center;
        float   _radius;
    };
}

#endif
