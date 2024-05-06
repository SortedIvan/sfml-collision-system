#pragma once
#include "SFML/Graphics.hpp"

float vectorLength(const sf::Vector2f vec)
{
	return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}
