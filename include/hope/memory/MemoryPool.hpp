#pragma once

namespace hope {
namespace mem {

class MemoryPool {
public:
    MemoryPool() : _elements(100) {}

    template<typename T>
    void * getElementMem(Id index) {
        if(index > _elements.size()) {
            _elements.reserve(_elements.size() + 100);
        }

        return static_cast<T*>(_elements[index]);
    }

    template<typename T>
    void deleteElement(Id index) {
        if(index > _elements.size()) { return; }
        T * ptr = static_cast<T*>(_elements[index]);
        ptr->~T();
    }
private:
    std::vector<void*> _elements;
};

}
}