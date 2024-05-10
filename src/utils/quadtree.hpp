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

        TL_BOUNDS.construct(boundary.x, boundary.y, boundary.w / 2, boundary.h / 2);
        TR_BOUNDS.construct(boundary.x + boundary.w / 2, boundary.y, boundary.w / 2, boundary.h / 2);
        BL_BOUNDS.construct(boundary.x, boundary.y + boundary.h / 2, boundary.w / 2, boundary.h / 2);
        BR_BOUNDS.construct(boundary.x + boundary.w / 2, boundary.y + boundary.h / 2, boundary.w / 2, boundary.h / 2);
        
        TL = std::make_unique<QuadNode>();
        TL->boundary = TL_BOUNDS;

        TR = std::make_unique<QuadNode>();
        TR->boundary = TR_BOUNDS;

        BL = std::make_unique<QuadNode>();
        BL->boundary = BL_BOUNDS;

        BR = std::make_unique<QuadNode>();
        BR->boundary = BR_BOUNDS;
    }

    void insert(sf::Vector2f point)
    {
        if (!boundary.contains(point.x, point.y))
        {
            // this is not the correct quadrant
            return;
        }

        if (points.size() < nodeCapacity && !hasBeenSplit)
        {
            // If the current node hasn't been split and still has capacity, add the point
            points.push_back(point);
        }
        else
        {
            if (!hasBeenSplit)
            {
                // If the current node hasn't been split, split it first
                split();

                // Redistribute existing points to child nodes
                for (const auto& p : points)
                {
                    TL->insert(p);
                    TR->insert(p);
                    BL->insert(p);
                    BR->insert(p);
                }
                points.clear(); // Clear the points in the current node
            }

            // Then insert the new point into the appropriate child node
            TL->insert(point);
            TR->insert(point);
            BL->insert(point);
            BR->insert(point);
        }
    }
};

void visualizeTree(std::unique_ptr<QuadNode>& root, sf::RenderWindow& window)
{
    if (!root)
    {
        return;
    }

    QuadRect bound = root.get()->boundary;

    sf::RectangleShape rect(sf::Vector2f(bound.w, bound.h));
    rect.setPosition(sf::Vector2f(bound.x, bound.y));
    rect.setOutlineThickness(1.f);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::White);

    window.draw(rect);

    for (int i = 0; i < root.get()->points.size(); ++i)
    {
        sf::CircleShape point(3.f);
        point.setPosition(root.get()->points[i]);
          
        window.draw(point);
    }

    visualizeTree(root.get()->TL, window);
    visualizeTree(root.get()->TR, window);
    visualizeTree(root.get()->BL, window);
    visualizeTree(root.get()->BR, window);
}

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