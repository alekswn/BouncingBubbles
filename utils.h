#ifndef UTILS_H
#define UTILS_H

#include <atomic>
#include <cassert>


//#define TRACE
#ifdef TRACE
#include <iostream>
#endif

template<typename T>
class SharedRW final
{
    std::atomic<T*> ptr;
    std::atomic_flag slaveWrite;

public:
    explicit SharedRW(const T& t)
       : ptr(new T(t)), slaveWrite(ATOMIC_FLAG_INIT)
    {
            assert(std::atomic_is_lock_free(&ptr));
    }
    ~SharedRW() {
        T*p = std::atomic_exchange(&ptr, static_cast<T*>(nullptr));
        if (p) delete p;
    }

    T get_slave() {
#ifdef TRACE
        std::cerr << __PRETTY_FUNCTION__ << " " << this << std::endl;
#endif
        slaveWrite.test_and_set();
        T* placeholder = nullptr;
        T* p = std::atomic_exchange(&ptr, placeholder);
        assert(p);
        T r = *p;
        bool succ = std::atomic_compare_exchange_strong(&ptr,&placeholder,p);
        if (!succ) delete p;
        return r;
    }

    bool set_slave(const T& val) {
#ifdef TRACE
        std::cerr << __PRETTY_FUNCTION__ << " " << this << std::endl;
#endif
        T* oldPtr = ptr.load();
        bool flag = slaveWrite.test_and_set();
        if (!flag) return false;
        T* newPtr = new T(val);
        bool succ = std::atomic_compare_exchange_strong(&ptr,&oldPtr,newPtr);
        if (!succ) delete newPtr;
        else if (oldPtr) delete oldPtr;
        return succ;
    }
    void set_master(const T& val) {
#ifdef TRACE
        std::cerr << __PRETTY_FUNCTION__ << " " << this << std::endl;
#endif
        slaveWrite.clear();
        T* newPtr = new T(val);
        T* oldPtr = atomic_exchange(&ptr, newPtr);
        if (oldPtr) delete oldPtr;
    }
};


#endif // UTILS_H
