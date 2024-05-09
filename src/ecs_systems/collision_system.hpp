#pragma once
#include "../ecs_components/components.hpp"
#include "../ecs_db/ecs_db.hpp"

class CollisionSystem 
{
    public:
        void handleCollisions(EcsDb& db);
        bool simpleCollisionCheck(const ColliderComponent& a, const ColliderComponent& b);
        void resolveCollision(ColliderComponent& a, ColliderComponent& b, EcsDb& db);
};