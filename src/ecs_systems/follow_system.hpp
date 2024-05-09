#pragma once
#include "../ecs_components/components.hpp"
#include "../ecs_db/ecs_db.hpp"

class FollowSystem
{
	private:
		bool isActivated = false;
	public:
		void activateFollow();
		void setFollowTarget(EcsDb& db, sf::Vector2f mousePos);
		bool getIsActivated();
};
