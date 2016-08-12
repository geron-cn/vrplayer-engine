#include <sys/time.h>
#include "Platform.h"

namespace vrlive {
    double Platform::getMachTimeInMilliseconds()
    {
        timeval tv;    
        gettimeofday(&tv,NULL);    
        return tv.tv_sec * 1000 + tv.tv_usec / 1000;  
    }
}
