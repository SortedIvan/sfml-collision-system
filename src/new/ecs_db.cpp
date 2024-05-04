#include "ecs_db.hpp"

TransformComponent* findTransformComponent(EcsDb& db, uint64_t entity_id)
{
	for (auto& transform : db.transformComponents)
	{
		if (transform.entity_id == entity_id)
		{
			return &transform;
		}
	}
	return nullptr;
}

ShapeComponent* findShapeComponent(EcsDb& db, uint64_t entity_id)
{
	for (auto& shape : db.shapeComponents)
	{
		if (shape.entity_id == entity_id)
		{
			return &shape;
		}
	}
	return nullptr;
}