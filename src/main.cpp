#include <hope/System.hpp>
#include <hope/SystemsManager.hpp>
#include <iostream>


using namespace hope;
struct A {};
struct B {};
struct C {};

struct SA : public hope::System {
    void update(EntityManager &entities, TimeDt dt)  {} 
};


int main() {
    hope::SystemsManager manager;

    hope::Entity entity = manager.entities().createEntityWithComponents(A{}, B{});
    ComponentHandler<C> a = entity.getComponent<C>();
    std::cout << "fodasse" << std::endl;
    return 0;
}