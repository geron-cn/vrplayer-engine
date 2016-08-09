#import <mach/mach_time.h>
#include "Platform.h"

namespace vrlive {
    double Platform::getMachTimeInMilliseconds()
    {
        static const double kOneMillion = 1000 * 1000;
        static mach_timebase_info_data_t s_timebase_info;
        
        if (s_timebase_info.denom == 0)
            (void) mach_timebase_info(&s_timebase_info);
        
        // mach_absolute_time() returns billionth of seconds, so divide by one million to get milliseconds
        (s_timebase_info.denom);
        return ((double)mach_absolute_time() * (double)s_timebase_info.numer) / (kOneMillion * (double)s_timebase_info.denom);
    }
}
