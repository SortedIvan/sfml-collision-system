#include "transform_system.hpp"
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


void TransformSystem::moveAllComponents(EcsDb& db, float deltaTime, float screenWidth, float screenHeight)
{
    for (auto& transform : db.transformComponents)
    {
        if (transform.isActive)
        {
            transform.previousPosition = transform.position;
            transform.acceleration *= transform.dampingFactor;
            transform.velocity += transform.acceleration * sf::Vector2f(1.f, 1.f) * deltaTime;
            transform.position = transform.position + transform.velocity;

            bool bounced = false;

            // Check for collision with screen boundaries
            if (transform.position.x < 0) {
                transform.position.x = 0;
                transform.velocity.x *= -1;
                bounced = true;
            }
            else if (transform.position.x > screenWidth) {
                transform.position.x = screenWidth;
                transform.velocity.x *= -1;
                bounced = true;
            }

            if (transform.position.y < 0) {
                transform.position.y = 0;
                transform.velocity.y *= -1;
                bounced = true;
            }
            else if (transform.position.y > screenHeight) {
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
