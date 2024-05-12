#pragma once
#include "../ecs_components/components.hpp"
#include "../ecs_db/ecs_db.hpp"
#include "../utils/quadtree.hpp"
#include <cmath>
#include <memory>

class TransformSystem
{
	public:
		void moveAllComponents(EcsDb& db, float deltaTime, float screenWidth, float screenHeight);
};