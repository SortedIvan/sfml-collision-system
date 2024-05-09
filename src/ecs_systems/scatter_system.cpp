#include "../ecs_systems/scatter_system.hpp"

float len(const sf::Vector2f vec)
{
	return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

bool checkIfPointInCircle(sf::Vector2f circleCenter, float radius, sf::Vector2f point)
{
	float lhs = (point.x - circleCenter.x) * (point.x - circleCenter.x)
		+ (point.y - circleCenter.y) * (point.y - circleCenter.y);

	if (lhs <= radius * radius)
	{
		return true;
	}

	return false;
}


void ScatterSystem::scatterObjects(sf::Vector2f mousePos, float radius, EcsDb& db)
{
	/*
		first, decide which objects are within the range of the circle
		the formula for a basic circle is
		(x - a)^2 + (x-b)^2 = r^2, where (a,b) is the center and r is the radius
	*/ 

	for (int i = 0; i < db.transformComponents.size(); i++)
	{
		if (checkIfPointInCircle(mousePos, radius, db.transformComponents[i].position))
		{
			sf::Vector2f newDir = db.transformComponents[i].position - mousePos;
			float length = len(newDir);

			db.transformComponents[i].velocity = sf::Vector2f((newDir.x / length) * 2500.f, (newDir.y / length) * 2500.f);
			db.transformComponents[i].isMoving = true;
		}
	}
}

