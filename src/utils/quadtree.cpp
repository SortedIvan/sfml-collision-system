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

    TL = std::make_unique<QuadNode>();
    TR = std::make_unique<QuadNode>();
    BL = std::make_unique<QuadNode>();
    BR = std::make_unique<QuadNode>();

    TL->boundary.construct(boundary.x, boundary.y, boundary.w / 2, boundary.h / 2);
    TR->boundary.construct(boundary.x + boundary.w / 2, boundary.y, boundary.w / 2, boundary.h / 2);
    BL->boundary.construct(boundary.x, boundary.y + boundary.h / 2, boundary.w / 2, boundary.h / 2);
    BR->boundary.construct(boundary.x + boundary.w / 2, boundary.y + boundary.h / 2, boundary.w / 2, boundary.h / 2);
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
//
//void QuadNode::update(sf::Vector2f oldPos, sf::Vector2f newPos, uint64_t transformId, std::unique_ptr<QuadNode>& root)
//{
//    if (!boundary.contains(oldPos.x, oldPos.y))
//    {
//        // this is not the correct quadrant
//
//        if (TL)
//        {
//            TL->update(oldPos, newPos, transformId, root);
//        }
//
//        if (TR)
//        {
//            TR->update(oldPos, newPos, transformId, root);
//        }
//
//        if (BL)
//        {
//            BL->update(oldPos, newPos, transformId, root);
//        }
//
//        if (BR)
//        {
//            BR->update(oldPos, newPos, transformId, root);
//        }
//    }
//    else
//    {   
//        int pointIndex = -1;
//        for (int i = 0; i < this->points.size(); i++)
//        {
//            if (this->points[i].transformId == transformId)
//            {
//                // we found the point
//                pointIndex = i;
//                break;
//            }
//        }
//
//        if (!pointIndex)
//        {
//            return;
//        }
//
//        // check if point contained in the same boundary so we don't need to move it
//        if (boundary.contains(newPos.x, newPos.y))
//        {
//            return;
//        }
//
//        QuadLeaf leaf = points[pointIndex]; 
//        leaf.position = newPos;
//
//        this->points.erase(this->points.begin() + pointIndex);
//
//        // Reinsert the point starting from the root
//        root->insert(leaf);
//
//        if (this->points.size() == 0 && !TL && !TR && !BL && !BR)
//        {
//            delete this;
//        }
//    }
//}

void QuadNode::update(sf::Vector2f oldPos, sf::Vector2f newPos, uint64_t transformId, const std::unique_ptr<QuadNode>& root) 
{
    if (!boundary.contains(oldPos.x, oldPos.y)) 
    {
        // this is not the correct quadrant
        if (TL) TL->update(oldPos, newPos, transformId, root);
        if (TR) TR->update(oldPos, newPos, transformId, root);
        if (BL) BL->update(oldPos, newPos, transformId, root);
        if (BR) BR->update(oldPos, newPos, transformId, root);

    }
    else
    {
        int pointIndex = -1;
        for (int i = 0; i < points.size(); i++) 
        {
            if (points[i].transformId == transformId) 
            {
                // we found the point
                pointIndex = i;
                break;
            }
        }

        if (pointIndex == -1) 
        {
            return;
        }

        // check if point contained in the same boundary so we don't need to move it
        if (boundary.contains(newPos.x, newPos.y)) 
        {
            return;
        }

        QuadLeaf& leaf = points[pointIndex];
        leaf.position = newPos;

        // Reinsert the point starting from the root
        root->insert(leaf);

        points.erase(points.begin() + pointIndex);

        if (points.size() == 0 && !TL && !TR && !BL && !BR) 
        {
            // no points and no children, can delete this node
            // it's managed by smart pointers
        }
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

    visualizeTree(root.get()->TL, window);
    visualizeTree(root.get()->TR, window);
    visualizeTree(root.get()->BL, window);
    visualizeTree(root.get()->BR, window);
}

void traverseQuadTree(std::unique_ptr<QuadNode>& root, std::string wentTo)
{
    if (!root)
    {
        return;
    }

    std::cout << wentTo << std::endl;

    for (int i = 0; i < root.get()->points.size(); i++)
    {
        std::cout << "(" << root.get()->points[i].position.x << ", " << root.get()->points[i].position.y << ")" << std::endl;
    }

    traverseQuadTree(root->TL, "TL");
    traverseQuadTree(root->TR, "TR");
    traverseQuadTree(root->BL, "BL");
    traverseQuadTree(root->BR, "BR");
}