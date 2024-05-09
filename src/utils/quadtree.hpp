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

    void construct(float x, float y, float w, float h)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    bool contains(float _x, float _y)
    {
        if (x <= _x && y <= _y && (x + w) >= _x && (y + h) >= _y)
        {
            return true;
        }
        return false;
    }
};

struct QuadNode
{
    std::unique_ptr<QuadNode> TL;
    std::unique_ptr<QuadNode> TR;
    std::unique_ptr<QuadNode> BL;
    std::unique_ptr<QuadNode> BR;

    int nodeCapacity = 4;
    QuadRect boundary;
    std::vector<sf::Vector2f> points;
    bool hasBeenSplit = false;

    void split()
    {
        hasBeenSplit = true;

        QuadRect TL_BOUNDS;
        QuadRect TR_BOUNDS;
        QuadRect BL_BOUNDS;
        QuadRect BR_BOUNDS;

        TL_BOUNDS.construct(boundary.x, boundary.y, boundary.x + boundary.w / 2, boundary.y + boundary.h / 2);
        TR_BOUNDS.construct(boundary.x + boundary.w / 2, boundary.y, boundary.x + boundary.w, boundary.y + boundary.h / 2);
        BL_BOUNDS.construct(boundary.x, boundary.y + boundary.h / 2, boundary.x + boundary.w / 2, boundary.y + boundary.h);
        BR_BOUNDS.construct(boundary.x + boundary.w / 2, boundary.y + boundary.h / 2, boundary.x + boundary.w, boundary.y + boundary.h);

        TL = std::make_unique<QuadNode>();
        TL.get()->boundary = TL_BOUNDS;

        TR = std::make_unique<QuadNode>();
        TR.get()->boundary = TR_BOUNDS;

        BL = std::make_unique<QuadNode>();
        BL.get()->boundary = BL_BOUNDS;

        BR = std::make_unique<QuadNode>();
        BR.get()->boundary = BR_BOUNDS;
    }

    void insert(sf::Vector2f point)
    {
        if (!boundary.contains(point.x, point.y))
        {
            // this is not the correct quadrant
            return;
        }

        if (points.size() != nodeCapacity)
        {
            points.push_back(point);
        }
        else
        {
            if (!hasBeenSplit)
            {
                split();
            }

            TL.get()->insert(point);
            TR.get()->insert(point);
            BL.get()->insert(point);
            BR.get()->insert(point);
        }
    }
};

void traverseQuadTree(std::unique_ptr<QuadNode>& root)
{
    if (!root)
    {
        return;
    }

    for (int i = 0; i < root.get()->points.size(); i++)
    {
        std::cout << "(" << root.get()->points[i].x << ", " << root.get()->points[i].y << ")" << std::endl;
    }

    traverseQuadTree(root->TL);
    traverseQuadTree(root->TR);
    traverseQuadTree(root->BL);
    traverseQuadTree(root->BR);
}