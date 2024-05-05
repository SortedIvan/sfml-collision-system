#include "click_system.hpp"
#include <iostream>
#include <random>


// TODO: Move all of this crap to be in utility libraries

const double PI = 3.1415926;

sf::Vector2f createRandomDirectionVector()
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 360);

	return sf::Vector2f(std::cos(dist6(rng)) * (PI / 180), std::sin(dist6(rng)) * (PI / 180));
}

float vectorLength(const sf::Vector2f vec)
{
	return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

sf::Color getRandomColor()
{
	// Create a random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 255); // Generate random numbers between 0 and 255

	// Generate random values for red, green, and blue components
	sf::Uint8 red = dis(gen);
	sf::Uint8 green = dis(gen);
	sf::Uint8 blue = dis(gen);

	// Return the random color
	return sf::Color(red, green, blue);
}

void ClickSystem::processClick(EcsDb& db, sf::Vector2f positionClicked)
{
	for (auto& click : db.clickableComponents)
	{
		if (click.isActive)
		{
			// check for clicks
		
			ShapeComponent* shape = findShapeComponent(db,click.entity_id);

			bool signSet = false;
			bool signToCompareTo = false; // left for false, right for true
			bool shapeClicked = true;

			for (int i = 0; i < shape->shape.getVertexCount(); i++)
			{
				sf::Vector2f polyEdge;

				if (i == shape->shape.getVertexCount() - 1)
				{
					// calculate the loop, aka with first vertex
					polyEdge = shape->shape[0].position - shape->shape[i].position;
				}
				else
				{
					polyEdge = shape->shape[i + 1].position - shape->shape[i].position;
				}

				sf::Vector2f pointEdge = positionClicked - shape->shape[i].position;
				
				bool sign = (polyEdge.x * pointEdge.y - pointEdge.x * polyEdge.y) > 0;

				// first edge checked, set the sign to compare the rest with
				if (!signSet) { 
					signToCompareTo = sign;
					signSet = true;
					continue;
				}

				if (sign != signToCompareTo) {
					shapeClicked = false;
					break;
				}

			}

			if (shapeClicked)
			{
				TransformComponent* transform = findTransformComponent(db, click.entity_id);

				sf::Color randColor = getRandomColor();

				for (int i = 0; i < shape->shape.getVertexCount(); i++)
				{
					shape->shape[i].color = randColor;
				}

				if (transform)
				{
					transform->velocity = createRandomDirectionVector() * 100.f;
				}
			}
		}
	}
}

