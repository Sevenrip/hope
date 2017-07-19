#include <hope/SystemsManager.hpp>
#include <iostream>


using namespace hope;
struct A { A(int i) : _i(i) {} int _i;};
struct B {};
struct C {};

struct SA : public hope::System {
    void update(EntityManager &entities, TimeDt dt)  {} 
};


int main() {
    hope::SystemsManager manager;

    hope::Entity entity = manager.entities().createEntityWithComponents(A{2}, B{}, C{});
    auto a = entity.component<A>();
    std::cout << a->_i << std::endl;
    return 0;
}