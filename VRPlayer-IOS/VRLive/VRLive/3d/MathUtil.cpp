#include "MathUtil.h"

namespace vrlive
{
    
    void MathUtil::smooth(float* x, float target, float elapsedTime, float responseTime)
    {
        
        if (elapsedTime > 0)
        {
            *x += (target - *x) * elapsedTime / (elapsedTime + responseTime);
        }
    }
    
    void MathUtil::smooth(float* x, float target, float elapsedTime, float riseTime, float fallTime)
    {
        
        if (elapsedTime > 0)
        {
            float delta = target - *x;
            *x += delta * elapsedTime / (elapsedTime + (delta > 0 ? riseTime : fallTime));
        }
    }
    
}