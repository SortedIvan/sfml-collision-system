#include "../ecs_systems/collision_system.hpp"

/*
    TO DO: Have this run with collider components, don't touch shape components
    this would require to update the collider positions every frame, not worth for simulation
    This also heavily assumes that the sf::VertexArray's are quads, 4 vertices each        
*/ 
void CollisionSystem::handleCollisions(EcsDb& db,  std::unique_ptr<QuadNode>& root) 
{
    for (int i = 0; i < db.shapeComponents.size(); i++)
    {
        QuadRect rect;

        rect.construct(
            db.shapeComponents[i].shape[0].position.x,
            db.shapeComponents[i].shape[0].position.y,
            WIDTH_RECT,
            HEIGHT_RECT
        );

        std::vector<QuadLeaf> leavesToCheck;
        std::map<uint64_t, int> alreadyChecked;
        queryRectCollision(rect, db.shapeComponents[i].shape_id, root, leavesToCheck, alreadyChecked);
        
        if (leavesToCheck.size() == 0)
        {
            // no point in calculating anything further
            continue;
        }

        // calculate the axis for the rect we are comparing the rest with
        // 4 edges => 4 normals that we store

        std::vector<sf::Vector2f> rectA_normals;
        
        calculateNormalsOfRectEdges(db.shapeComponents[i].shape, rectA_normals);
      
        for (int k = 0; k < leavesToCheck.size(); k++)
        {
            std::vector<sf::Vector2f> rectB_normals;
            calculateNormalsOfRectEdges(db.shapeComponents[db.shapeComponentMapper[leavesToCheck[k].shape_id]].shape, rectB_normals);
            
            bool collision = true;

            for (int j = 0; j < rectA_normals.size(); j++)
            {
                sf::Vector2f minMax_A = getMinMaxProjectionFromVertices(rectA_normals[j], db.shapeComponents[i].shape);
                sf::Vector2f minMax_B = getMinMaxProjectionFromVertices(rectA_normals[j], db.shapeComponents[db.shapeComponentMapper[leavesToCheck[k].shape_id]].shape);
                
                if (minMax_A.x >= minMax_B.y || minMax_B.x >= minMax_A.y)
                {
                    collision = false;
                    break;
                }
            }

            if (collision)
            {
                for (int j = 0; j < rectB_normals.size(); j++)
                {
                    sf::Vector2f minMax_A = getMinMaxProjectionFromVertices(rectB_normals[j], db.shapeComponents[i].shape);
                    sf::Vector2f minMax_B = getMinMaxProjectionFromVertices(rectB_normals[j], db.shapeComponents[db.shapeComponentMapper[leavesToCheck[k].shape_id]].shape);

                    if (minMax_A.x >= minMax_B.y || minMax_B.x >= minMax_A.y)
                    {
                        collision = false;
                        break;
                    }
                }
            }
            
            if (collision)
            {
                // otherwise, collision between the leaf rect and the rect we are comparing with
                std::cout << "wow, collision" << std::endl;
            }
        }
    }
}

bool CollisionSystem::simpleCollisionCheck(const ColliderComponent& a, const ColliderComponent& b) {
    // Implement actual collision detection based on colliderBounds and types
    return false;
}

void CollisionSystem::resolveCollision(ColliderComponent& a, ColliderComponent& b, EcsDb& db) {
    // Implement collision resolution
    // Example: Change positions to avoid overlap or simulate physical response
    auto& transformA = *findTransformComponent(db, a.entity_id);
    auto& transformB = *findTransformComponent(db, b.entity_id);

    // Simple response: move back to previous positions to avoid overlap
    transformA.position = transformA.previousPosition;
    transformB.position = transformB.previousPosition;
}
