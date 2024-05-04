#pragma once;
#include "ecs_db.hpp"

class ClickSystem
{
	public:
		void processClick(EcsDb& db, sf::Vector2f positionClicked);
};