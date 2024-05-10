#include "quadtree.hpp"

void QuadRect::construct(float x, float y, float w, float h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

bool QuadRect::contains(float _x, float _y)
{
    if (x <= _x && y <= _y && (x + w) >= _x && (y + h) >= _y)
    {
        return true;
    }
    return false;
}

void QuadNode::split()
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

void QuadNode::insert(QuadLeaf element)
{
    if (!boundary.contains(element.position.x, element.position.y))
    {
        // this is not the correct quadrant
        return;
    }

    if (points.size() < nodeCapacity && !hasBeenSplit)
    {
        points.push_back(element);
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

            points.clear();
        }

        // Then insert the new point into the appropriate child node
        TL->insert(element);
        TR->insert(element);
        BL->insert(element);
        BR->insert(element);
    }
}


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

    //for (int i = 0; i < root.get()->points.size(); ++i)
    //{
    //    sf::CircleShape point(3.f);
    //    point.setPosition(root.get()->points[i]);
    //      
    //    window.draw(point);
    //}

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
        std::cout << "(" << root.get()->points[i].position.x << ", " << root.get()->points[i].position.y << ")" << std::endl;
    }

    traverseQuadTree(root->TL);
    traverseQuadTree(root->TR);
    traverseQuadTree(root->BL);
    traverseQuadTree(root->BR);
}