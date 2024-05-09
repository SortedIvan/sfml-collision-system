#include "../ecs_systems/follow_system.hpp"


void FollowSystem::activateFollow()
{
	isActivated = !isActivated;
}

float vectorLength(const sf::Vector2f vec)
{
	return std::sqrt(vec.x * vec.x + vec.y * vec.y);
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

			transform->velocity = sf::Vector2f(dirVector.x, dirVector.y) * 10.f;
		}
	}
}

bool FollowSystem::getIsActivated()
{
	return isActivated;
}