//
// Created by tony
//
#ifndef REF_H_
#define REF_H_

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
