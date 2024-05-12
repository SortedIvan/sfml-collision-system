#include "../ecs_systems/collision_system.hpp"

// TO DO: Have this run with collider components, don't touch shape components
// this would require to update the collider positions every frame, not worth for simulation
void CollisionSystem::handleCollisions(EcsDb& db, std::unique_ptr<QuadNode>& root) 
{
    for (int i = 0; i < db.shapeComponents.size(); i++)
    {
        //root->
    }
}

bool CollisionSystem::simpleCollisionCheck(const ColliderComponent& a, const ColliderComponent& b) {
    // Implement actual collision detection based on colliderBounds and types
    return false;
}

void CollisionSystem::resolveCollision(ColliderComponent& a, ColliderComponent& b, EcsDb& db) {
    // Implement collision resolution
    // Example: Change positions to avoid overlap or simulate physical response
    auto& transformA = *findTransformComponent(db, a.entity_id);
    auto& transformB = *findTransformComponent(db, b.entity_id);

    // Simple response: move back to previous positions to avoid overlap
    transformA.position = transformA.previousPosition;
    transformB.position = transformB.previousPosition;
}
