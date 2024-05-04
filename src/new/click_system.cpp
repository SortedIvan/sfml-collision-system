#include "click_system.hpp"
#include <iostream>

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
				// perform click action here
				std::cout << "Clicked";
			}
		}
	}
}

