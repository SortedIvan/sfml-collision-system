#include "transform_system.hpp"
#include <iostream>

void TransformSystem::moveAllComponents(EcsDb& db, float deltaTime) 
{
	for (auto& transform : db.transformComponents)
	{
		if (transform.isActive)
		{
            // apply all movement here
           // Record the old position
            transform.previousPosition = transform.position;

            // Update the acceleration
            transform.acceleration *= transform.dampingFactor;

            // Update the velocity with the now updated accel
            transform.velocity += transform.acceleration * sf::Vector2f(1.f, 1.f) * deltaTime;

            // finally, add the velocity to the position
            transform.position = transform.position + transform.velocity;

            // check if the component had a shape component that we need to update

            ShapeComponent* shape = findShapeComponent(db, transform.entity_id);
            
            if (shape)
            {
                shape->positionHasChanged = true;
            }
           
		}
	}
}


