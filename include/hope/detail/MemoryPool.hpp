#pragma once

namespace hope {
namespace detail {

class BaseMemoryPool {
public:
    BaseMemoryPool(size_t typeSize) : _typeSize(typeSize), _memory(new char[1024]) {};

    void * getElementMem(Id index) {
        auto memOffset = index * _typeSize;
        return _memory + memOffset;
    }

    virtual void deleteElement(Id index) = 0;

protected:
    size_t _typeSize;
    char * _memory;
};

template<typename T>
class MemoryPool : public BaseMemoryPool {
public:
    MemoryPool() : BaseMemoryPool(sizeof(T)) {}

    virtual void deleteElement(Id index) final {
        auto memOffset = index * _typeSize;
        T * ptr = reinterpret_cast<T*>(_memory + memOffset);
        ptr->~T();
    }
};

}
}