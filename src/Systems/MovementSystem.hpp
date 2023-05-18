#ifndef MOVEMENTSYSTEM_HPP
#define MOVEMENTSYSTEM_HPP

#include "../ECS/ECS.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include "../Logger/Logger.hpp"

class MovementSystem: public System {
    public:
        MovementSystem() {
            requireComponent<TransformComponent>();
            requireComponent<RigidBodyComponent>();
        }

        void update(double deltaTime) {
            for (auto entity : getSystemEntities()) {
                auto& transform = entity.getComponent<TransformComponent>();
                const auto rigidbody = entity.getComponent<RigidBodyComponent>();
                transform.position.x += rigidbody.velocity.x * deltaTime;
                transform.position.y += rigidbody.velocity.y * deltaTime;
//                Logger::Log("Changing position to " + std::to_string(transform.position.x) + "." + std::to_string(transform.position.y));
            }
        }
};
#endif
