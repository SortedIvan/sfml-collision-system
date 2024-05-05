#pragma once
#include "components.hpp"
#include "ecs_db.hpp"

class FollowSystem
{
	private:
		bool isActivated;
	public:
		void activateFollow();
		void follow(EcsDb& db, sf::Vector2f mousePos);
};
