#include "../ecs_systems/transform_system.hpp"
#include <iostream>
#include <random>

sf::Color genRandomColor()
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

void TransformSystem::moveAllComponents(EcsDb& db, float deltaTime, float screenWidth, float screenHeight, std::unique_ptr<QuadNode>& root)
{
    for (auto& transform : db.transformComponents)
    {
        if (transform.isActive)
        {
            if (!transform.isMoving)
            {
                continue;
            }

            transform.previousPosition = transform.position;

            // Update velocity based on acceleration
            transform.velocity += transform.acceleration * deltaTime;

            // Update position based on velocity
            transform.position += transform.velocity * deltaTime;
            
            /*
                Use the previousPosition to query the tree.
                Then, use the newPosition to update the point
            */

            if (root->TL)
            {
                root->TL->update(transform.previousPosition, transform.position, transform.transform_id, root);
            }

            if (root->TR)
            {
                root->TR->update(transform.previousPosition, transform.position, transform.transform_id, root);
            }

            if (root->BL)
            {
                root->BL->update(transform.previousPosition, transform.position, transform.transform_id, root);
            }

            if (root->BR)
            {
                root->BR->update(transform.previousPosition, transform.position, transform.transform_id, root);
            }


            // Apply damping to velocity
            transform.velocity.x -= transform.velocity.x * transform.dampingFactor * deltaTime;
            transform.velocity.y -= transform.velocity.y * transform.dampingFactor * deltaTime;

            if (std::abs(transform.velocity.x) < 6.f) 
            {
                transform.velocity.x = 0.0000f;
                transform.isMoving = false;
            }
            if (std::abs(transform.velocity.y) < 6.f) 
            {
                transform.velocity.y = 0.0000f;
                transform.isMoving = false;
            }
            
         
            bool bounced = false;

            // Check for collision with screen boundaries
            if (transform.position.x < 0) {
                transform.position.x = 0;
                transform.velocity.x *= -1;
                bounced = true;
            }
            else if (transform.position.x > screenWidth) 
            {
                transform.position.x = screenWidth;
                transform.velocity.x *= -1;
                bounced = true;
            }

            if (transform.position.y < 0) {
                transform.position.y = 0;
                transform.velocity.y *= -1;
                bounced = true;
            }
            else if (transform.position.y > screenHeight) 
            {
                transform.position.y = screenHeight;
                transform.velocity.y *= -1;
                bounced = true;
            }

            // Check if the component had a shape component that we need to update
            ShapeComponent* shape = findShapeComponent(db, transform.entity_id);
            if (shape)
            {
                shape->positionHasChanged = true;

                if (bounced)
                {
                    sf::Color randColor = genRandomColor();

                    for (int i = 0; i < shape->shape.getVertexCount(); i++)
                    {
                        shape->shape[i].color = randColor;
                    }
                }

            }

        }
    }
}
