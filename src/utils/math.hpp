#pragma once
#include "SFML/Graphics.hpp"
#include <random>

float vectorLength(const sf::Vector2f vec)
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

sf::Vector2f randomVelocity(int scalar)
{
	const double PI = 3.1415926;

	std::random_device dev;
	std::mt19937 rng(dev());

	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 360);
	return sf::Vector2f(std::cos(dist6(rng)) * (PI / 180) * scalar, std::sin(dist6(rng)) * (PI / 180) * scalar);
}


