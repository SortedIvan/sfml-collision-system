#include "math.hpp"

float veclen(const sf::Vector2f vec)
{
	return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

bool checkIfPointInCirc(sf::Vector2f circleCenter, float radius, sf::Vector2f point)
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

float dot2(sf::Vector2f a, sf::Vector2f b)
{
	return a.x * b.x + a.y + b.y;
}

float len2(sf::Vector2f a)
{
	return std::sqrt(a.x * a.x + a.y * a.y);
}

sf::Vector2f normal2(sf::Vector2f a)
{
	return sf::Vector2f(-a.y, a.x);
}

/*
	
   A--------B
	|      |
	|      |
	|      |
   D--------C

*/
void calculateNormalsOfRectEdges(sf::VertexArray& rect, std::vector<sf::Vector2f>& output)
{
	sf::Vector2f AB = rect[1].position - rect[0].position;
	sf::Vector2f BC = rect[2].position - rect[1].position;
	sf::Vector2f CD = rect[3].position - rect[2].position;
	sf::Vector2f AD = rect[3].position - rect[0].position;

	output = { normal2(AB), normal2(BC), normal2(CD), normal2(AD)};
}


sf::Vector2f getMinMaxProjectionFromVertices(sf::Vector2f axis, sf::VertexArray vertices)
{
	float max = std::numeric_limits<float>::lowest();
	float min = std::numeric_limits<float>::max();

	for (int i = 0; i < vertices.getVertexCount(); i++)
	{
		// project and find the min and max
		float vertexProjected = dot2(vertices[i].position, axis);

		if (vertexProjected > max)
		{
			max = vertexProjected;
		}

		if (vertexProjected < min)
		{
			min = vertexProjected;
		}
	}

	return sf::Vector2f(min, max);
}