#include "click_system.hpp"
#include <iostream>
#include <random>

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

				if (transform)
				{
					transform->velocity = createRandomDirectionVector() * 30.f;
				}
			}
		}
	}
}

