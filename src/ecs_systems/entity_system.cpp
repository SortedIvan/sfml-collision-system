#include "../ecs_systems/entity_system.hpp"

uint64_t EntitySystem::createEntity(EcsDb& db)
{
    uint64_t entityID;
    if (!db.freeList.empty()) 
    {
        entityID = db.freeList.top();
        db.freeList.pop();
    }
    else 
    {
        entityID = db.entityCounter++;
    }

    db.entities.push_back(entityID);
    return entityID;
}

TransformComponent& EntitySystem::addTransformComponent(EcsDb& db, uint64_t entityID) 
{
    db.transformComponents.emplace_back();
    TransformComponent& component = db.transformComponents.back();
    component.entity_id = entityID;
    component.isActive = true;
    return component;
}

ColliderComponent& EntitySystem::addColliderComponent(EcsDb& db, uint64_t entityID) 
{
    db.colliderComponents.emplace_back();
    ColliderComponent& component = db.colliderComponents.back();
    component.entity_id = entityID;
    component.isActive = true;
    return component;
}

ShapeComponent& EntitySystem::addShapeComponent(EcsDb& db, uint64_t entityID) 
{
    db.shapeComponents.emplace_back();
    ShapeComponent& component = db.shapeComponents.back();
    component.entity_id = entityID;
    component.isActive = true;
    return component;
}

ClickableComponent& EntitySystem::addClickableComponent(EcsDb& db, uint64_t entityID)
{
    db.clickableComponents.emplace_back();
    ClickableComponent& component = db.clickableComponents.back();
    component.entity_id = entityID;
    component.isActive = true;
    return component;
}

FollowMousePosComponent& EntitySystem::addFollowMouseComponent(EcsDb& db, uint64_t entityID)
{
    db.followMouseComponents.emplace_back();
    FollowMousePosComponent& component = db.followMouseComponents.back();
    component.entity_id = entityID;
    component.isActive = true;
    return component;
}