#pragma once
#include "components.hpp"
#include "ecs_db.hpp"


class TransformSystem
{
	public:
		void moveAllComponents(EcsDb& db, float deltaTime);
};