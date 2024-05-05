#include "follow_system.hpp"

void FollowSystem::activateFollow()
{
	isActivated = !isActivated;
}

void FollowSystem::follow(EcsDb& db, sf::Vector2f mousePos)
{
	if (!isActivated)
	{
		return;
	}

	for (int i = 0; i < db.followMouseComponents.size(); i++)
	{
	//	db.followMouseComponents[i].
	}
}
