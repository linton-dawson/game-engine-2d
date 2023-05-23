#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include "../ECS/ECS.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/TransformComponent.hpp"

class CollisionSystem : public System {
    public:
        CollisionSystem() {
            requireComponent<BoxColliderComponent>();
            requireComponent<TransformComponent>();
        }

        bool checkAABBCollision(double ax, double ay, double aw, double ah, double bx, double by, double bw, double bh) {
            return(ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by);
        }

        void update() {
            auto entities = getSystemEntities();
            for(auto it = entities.begin(); it != entities.end(); ++it) {
                auto a = *it;
                auto aTransform = a.getComponent<TransformComponent>();
                auto aCollider = a.getComponent<BoxColliderComponent>();
                for(auto jt = it + 1 ; jt != entities.end() ; ++jt) {
                    auto b = *jt;
                    auto bTransform = b.getComponent<TransformComponent>();
                    auto bCollider = b.getComponent<BoxColliderComponent>();
                    bool collides = checkAABBCollision(aTransform.position.x + aCollider.offset.x, aTransform.position.y + aCollider.offset.y, aCollider.w, aCollider.h,
                                                        bTransform.position.x + bCollider.offset.x, bTransform.position.y + bCollider.offset.y, bCollider.w, bCollider.h);

                    if(collides) {
                        Logger::Log("Entity " + std::to_string(a.getId()) + " collides with " + std::to_string(b.getId()));
                    }
                }
            }
        }
};
#endif
