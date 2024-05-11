#include "../ecs_db/ecs_db.hpp"

class EntitySystem
{
    private:
        int transformIdCounter = 0;
    public:
        uint64_t createEntity(EcsDb& db);
        TransformComponent& addTransformComponent(EcsDb& db, uint64_t entityID);
        ColliderComponent& addColliderComponent(EcsDb& db, uint64_t entityID);
        ShapeComponent& addShapeComponent(EcsDb& db, uint64_t entityID);
        ClickableComponent& addClickableComponent(EcsDb& db, uint64_t entityID);
        FollowMousePosComponent& addFollowMouseComponent(EcsDb& db, uint64_t entityID);
};