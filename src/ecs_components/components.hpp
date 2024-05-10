#pragma once
#include "SFML/Graphics.hpp"
#include <stdint.h>
#include <stack>


enum ColliderType 
{
    BoxCollider,
    TriangleCollider,
    CircleCollider,
    CapsuleCollider
};

// ========== Collision =================
struct ColliderComponent 
{
    sf::VertexArray colliderBounds;
    ColliderType colliderType;
    uint64_t entity_id;
    bool isActive;
};

// ======== Transform ===================
struct TransformComponent
{
    sf::Vector2f position;
    sf::Vector2f previousPosition;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    uint64_t entity_id;
    uint64_t transform_id; // used for identifying in quad tree
    float dampingFactor;
    bool isActive;
    bool isMoving = false;
};

// ======== Shapes =======================
struct ShapeComponent
{
    sf::VertexArray shape;
    uint64_t entity_id;
    bool isActive;
    bool positionHasChanged = false;
};

// ======== Interactable =================
struct ClickableComponent
{
    bool wasClicked = false;
    uint64_t entity_id;
    bool isActive;
};

struct FollowMousePosComponent
{
    uint64_t entity_id;
    bool isActive;
};