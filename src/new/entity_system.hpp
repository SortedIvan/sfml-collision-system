#include "ecs_db.hpp"

class EntitySystem
{
    public:
        uint64_t createEntity(EcsDb& db);
        TransformComponent& addTransformComponent(EcsDb& db, uint64_t entityID);
        ColliderComponent& addColliderComponent(EcsDb& db, uint64_t entityID);
        ShapeComponent& addShapeComponent(EcsDb& db, uint64_t entityID);
};