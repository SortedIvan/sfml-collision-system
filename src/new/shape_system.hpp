#pragma once
#include "components.hpp"
#include "ecs_db.hpp"

class ShapeSystem
{
	public:
		void drawShapes(EcsDb& db, sf::RenderWindow& window);
		void moveShapesIfNeeded(EcsDb& db);
};
