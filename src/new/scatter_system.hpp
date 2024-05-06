#pragma once
#include "SFML/Graphics.hpp"
#include "ecs_db.hpp"

class ScatterSystem
{
	public:
		void scatterObjects(sf::Vector2f mousePos, float radius, EcsDb& db);
};