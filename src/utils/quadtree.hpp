#pragma once
#include <vector>
#include <iostream>
#include "SFML/Graphics.hpp"

struct QuadRect
{
    float x;
    float y;
    float w;
    float h;

    void construct(float x, float y, float w, float h);
    bool contains(float _x, float _y);
};

struct QuadLeaf
{
    sf::Vector2f position;
    uint64_t transformId;
};

struct QuadNode
{
    std::unique_ptr<QuadNode> TL;
    std::unique_ptr<QuadNode> TR;
    std::unique_ptr<QuadNode> BL;
    std::unique_ptr<QuadNode> BR;

    int nodeCapacity = 4;
    QuadRect boundary;
    std::vector<QuadLeaf> points;
    bool hasBeenSplit = false;

    void split();
    void insert(QuadLeaf element);
};

void visualizeTree(std::unique_ptr<QuadNode>& root, sf::RenderWindow& window);
void traverseQuadTree(std::unique_ptr<QuadNode>& root);