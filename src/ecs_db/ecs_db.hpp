/*
	Central db for all components and entities
	Holds entities, components
*/
#pragma once
#include <vector>
#include "../ecs_components/components.hpp"
#include <map>

struct EcsDb
{
	// ============== Components ======================
	std::vector<TransformComponent> transformComponents;
	std::vector<ShapeComponent> shapeComponents;
	std::vector<ColliderComponent> colliderComponents;
	std::vector<ClickableComponent> clickableComponents;
	std::vector<FollowMousePosComponent> followMouseComponents;

	// TODO: Rework this, leave for testing purposes
	std::map<uint64_t, int> shapeComponentMapper;

	// ============== Entities ========================
	std::vector<uint64_t> entities;
	std::stack<uint64_t> freeList;  // Stack to hold available (deleted) entity IDs
	uint64_t entityCounter = 0;
};

TransformComponent* findTransformComponent(EcsDb& db, uint64_t entity_id);
ShapeComponent* findShapeComponent(EcsDb& db, uint64_t entity_id);