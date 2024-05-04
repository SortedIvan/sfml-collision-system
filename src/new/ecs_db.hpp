/*
	Central db for all components and entities
	Holds entities, components
*/
#pragma once
#include <vector>
#include "components.hpp"

struct EcsDb
{
	// ============== Components ======================
	std::vector<TransformComponent> transformComponents;
	std::vector<ShapeComponent> shapeComponents;
	std::vector<ColliderComponent> colliderComponents;
	std::vector<ClickableComponent> clickableComponents;

	// ============== Entities ========================
	std::vector<uint64_t> entities;
	std::stack<uint64_t> freeList;  // Stack to hold available (deleted) entity IDs
	uint64_t entityCounter = 0;
};

TransformComponent* findTransformComponent(EcsDb& db, uint64_t entity_id);
ShapeComponent* findShapeComponent(EcsDb& db, uint64_t entity_id);