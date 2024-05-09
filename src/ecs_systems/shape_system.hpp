#pragma once
#include "../ecs_components/components.hpp"
#include "../ecs_db/ecs_db.hpp"

class ShapeSystem
{
	public:
		void drawShapes(EcsDb& db, sf::RenderWindow& window);
		void moveShapesIfNeeded(EcsDb& db);
};
