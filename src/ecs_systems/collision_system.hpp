#pragma once
#include <vector>
#include <map>
#include "../ecs_components/components.hpp"
#include "../ecs_db/ecs_db.hpp"
#include "../utils/quadtree.hpp"
#include "../utils/globals.hpp"
#include "../utils/math.hpp"

class CollisionSystem 
{
    public:
        void handleCollisions(EcsDb& db, std::unique_ptr<QuadNode>& root);
        bool simpleCollisionCheck(const ColliderComponent& a, const ColliderComponent& b);
        void resolveCollision(ColliderComponent& a, ColliderComponent& b, EcsDb& db);
};