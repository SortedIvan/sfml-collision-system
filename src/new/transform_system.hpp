#pragma once
#include "components.hpp"
#include "ecs_db.hpp"
#include <cmath>

class TransformSystem
{
	public:
		void moveAllComponents(EcsDb& db, float deltaTime);
};