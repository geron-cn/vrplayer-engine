//
// Created by tony
//
#ifndef REF_H_
#define REF_H_
#ifdef __ANDROID__
#include <stdlib.h>
#endif // __ANDROID__

namespace vrlive {
    class Ref
    {
    public:
        void addRef();
        
        void release();
        
        int getRefCount() const;
        
    protected:
        Ref();
        Ref(const Ref& copy);
        virtual ~Ref();
        
    private:
        unsigned int _refCount;
    };
}

#endif
