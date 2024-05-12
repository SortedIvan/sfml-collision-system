#pragma once
#include "SFML/Graphics.hpp"
#include <random>
#include <vector>

float veclen(const sf::Vector2f vec);
bool checkIfPointInCirc(sf::Vector2f circleCenter, float radius, sf::Vector2f point);
sf::Vector2f randomVelocity(int scalar);
void calculateNormalsOfRectEdges(sf::VertexArray& rect, std::vector<sf::Vector2f>& output);
sf::Vector2f normal2(sf::Vector2f a);
float len2(sf::Vector2f a);
float dot2(sf::Vector2f a, sf::Vector2f b);
sf::Vector2f getMinMaxProjectionFromVertices(sf::Vector2f axis, sf::VertexArray vertices);