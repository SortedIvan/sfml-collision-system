#pragma once
#include <vector>
#include <iostream>
#include "SFML/Graphics.hpp"
#include <string>

struct QuadRect
{
    float x;
    float y;
    float w;
    float h;

    void construct(float x, float y, float w, float h);
    bool containsPoint(float _x, float _y);
};

struct QuadLeaf
{
    sf::Vector2f position; // position of top left corner of quad
    sf::Vector2f size;     // width, height
    uint64_t shape_id;
};

/*
    Currently, tree is being re-built every frame for test purposes
    This allows us to directly insert rectangles that overlap multiple areas since tree will be destroyed anyways
*/
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
    void update(sf::Vector2f oldPos, sf::Vector2f newPos, uint64_t shape_id, std::unique_ptr<QuadNode>& prev, std::unique_ptr<QuadNode>& root);

};

void visualizeTree(std::unique_ptr<QuadNode>& root, sf::RenderWindow& window);
void traverseQuadTree(std::unique_ptr<QuadNode>& root, std::string wentTo, int level);
void remove(std::unique_ptr<QuadNode>& parent, QuadNode* child);
void queryRectangle(const QuadRect& rect, std::unique_ptr<QuadNode>& root);