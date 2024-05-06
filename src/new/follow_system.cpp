#include "follow_system.hpp"
#include "math.hpp"

void FollowSystem::activateFollow()
{
	isActivated = !isActivated;
}

void FollowSystem::setFollowTarget(EcsDb& db, sf::Vector2f mousePos)
{
	if (!isActivated)
	{
		return;
	}

	for (int i = 0; i < db.followMouseComponents.size(); i++)
	{
		TransformComponent* transform = findTransformComponent(db, db.followMouseComponents[i].entity_id);

		if (transform)
		{
			sf::Vector2f dirVector = mousePos - transform->position;

			float dirLen = vectorLength(dirVector);
			float velocityLen = vectorLength(transform->velocity);

			transform->velocity = sf::Vector2f(dirVector.x / dirLen, dirVector.y / dirLen) * velocityLen;
		}
	}
}
