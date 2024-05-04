#include "shape_system.hpp"

void ShapeSystem::drawShapes(EcsDb& db, sf::RenderWindow& window)
{
    for (auto& shape : db.shapeComponents)
    {
        if (shape.isActive)
        {
            window.draw(shape.shape);
        }
    }
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