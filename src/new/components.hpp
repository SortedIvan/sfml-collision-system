#pragma once
#include "SFML/Graphics.hpp"
#include <stdint.h>
#include <stack>


enum ColliderType {
    BoxCollider,
    TriangleCollider,
    CircleCollider,
    CapsuleCollider
};

// ========== Collision =================
struct ColliderComponent {
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
    uint64_t entity_id;
    bool isActive;
    float acceleration;
    float dampingFactor;
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