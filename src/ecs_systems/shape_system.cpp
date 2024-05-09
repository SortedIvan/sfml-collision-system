#include "../ecs_systems/shape_system.hpp"
#include <thread>
#include <mutex>

void ShapeSystem::drawShapes(EcsDb& db, sf::RenderWindow& window)
{
    sf::VertexArray batchVertexArray(sf::Quads);

    for (auto& shape : db.shapeComponents)
    {
        if (shape.isActive)
        {
            for (int i = 0; i < shape.shape.getVertexCount(); i++)
            {
                batchVertexArray.append(shape.shape[i]);
            }
        }
    }

    // Draw the batched entities in a single draw call
    window.draw(batchVertexArray);
}

void ShapeSystem::moveShapesIfNeeded(EcsDb& db)
{
    for (auto& shape : db.shapeComponents)
    {
        if (shape.isActive)
        {
            if (shape.positionHasChanged)
            {
                // update position here

                 TransformComponent* transform = findTransformComponent(db, shape.entity_id);

                 if (transform)
                 {
                     sf::Vector2f diff = transform->position - transform->previousPosition;

                     for (int i = 0; i < shape.shape.getVertexCount(); i++)
                     {
                         shape.shape[i].position += diff;
                     }
                 }
                 
                 shape.positionHasChanged = false;
            }
        }
    }
}