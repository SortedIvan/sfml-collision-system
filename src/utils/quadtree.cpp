#include "quadtree.hpp"


void QuadRect::construct(float x, float y, float w, float h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

bool QuadRect::containsPoint(float _x, float _y)
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

/*
    For testing purposes, insert the leaf in all boundaries that contain the rectangle
*/ 
void QuadNode::insert(QuadLeaf element)
{
    
    if (!boundary.containsPoint(element.position.x, element.position.y) &&
        !boundary.containsPoint(element.position.x + element.size.x, element.position.y) &&
        !boundary.containsPoint(element.position.x + element.size.x, element.position.y + element.size.y) &&
        !boundary.containsPoint(element.position.x, element.position.y + element.size.y)) {
        // Element does not intersect with this node's boundary, so we cannot insert it here
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

void QuadNode::update(sf::Vector2f oldPos, sf::Vector2f newPos, uint64_t shape_id,std::unique_ptr<QuadNode>& prev, std::unique_ptr<QuadNode>& root)
{
    if (!boundary.containsPoint(oldPos.x, oldPos.y))
    {
        // this is not the correct quadrant
        if (TL) TL->update(oldPos, newPos, shape_id, TL, root);
        if (TR) TR->update(oldPos, newPos, shape_id, TR, root);
        if (BL) BL->update(oldPos, newPos, shape_id, BL, root);
        if (BR) BR->update(oldPos, newPos, shape_id, BR, root);
    }
    else
    {
        int pointIndex = -1;

        for (int i = 0; i < points.size(); i++) 
        {
            if (points[i].shape_id == shape_id)
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
        if (this->boundary.containsPoint(newPos.x, newPos.y))
        {
            return;
        }

        this->points.erase(this->points.begin() + pointIndex);

        if (points.empty())
        {
            remove(prev, this);
        }

        QuadLeaf& leaf = points[pointIndex];
        leaf.position = newPos;

        // Reinsert the point starting from the root
        root->insert(leaf);
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

    visualizeTree(root->TL, window);
    visualizeTree(root->TR, window);
    visualizeTree(root->BL, window);
    visualizeTree(root->BR, window);
}

void traverseQuadTree(std::unique_ptr<QuadNode>& root, std::string wentTo, int level)
{
    if (!root)
    {
        return;
    }

    std::string spacing = "";

    for (int i = 0; i < level; i++)
    {
        spacing += "  ";
    }
    
    std::cout << spacing << wentTo << std::endl;

    for (int i = 0; i < root.get()->points.size(); ++i)
    {
        std::cout << spacing << "  " << "(" << root.get()->points[i].position.x << ", " << root.get()->points[i].position.y << ")" << std::endl;
    }

    level++;

    traverseQuadTree(root->TL, "TL", level);
    traverseQuadTree(root->TR, "TR", level);
    traverseQuadTree(root->BL, "BL", level);
    traverseQuadTree(root->BR, "BR", level);
}

void remove(std::unique_ptr<QuadNode>& parent, QuadNode* child) 
{
    if (!parent) {
        return; // Parent doesn't exist
    }

    if (parent->TL.get() == child) {
        parent->TL.reset(); // Reset the unique pointer, removing the child
    }
    else if (parent->TR.get() == child) 
    {
        parent->TR.reset();
    }
    else if (parent->BL.get() == child) 
    {
        parent->BL.reset();
    }
    else if (parent->BR.get() == child) 
    {
        parent->BR.reset();
    }
}

/*
    Fetches all of the potential colliders
    Could be duplicates, would still have to check them
    This means we have to check collision redundantly
    For some memory tradeoff, we can have a map to reduce this
*/
void queryRectCollision(const QuadRect& rect, std::unique_ptr<QuadNode>& root, std::vector<QuadNode>& toCheck, std::map<uint64_t, int>& alreadyChecked)
{
    if (!root->boundary.containsPoint(rect.x, rect.y) &&
        !root->boundary.containsPoint(rect.x + rect.w, rect.y) &&
        !root->boundary.containsPoint(rect.x + rect.w, rect.y + rect.h) &&
        !root->boundary.containsPoint(rect.x, rect.y + rect.h)) 
    {
        // Element does not intersect with this node's boundary, so we stop querying
        return;
    }

    // otherwise, we are in a correct segment
    // However, we still need to crawl as deep as possible

    if (root->hasBeenSplit)
    {
        // go deeper
    }
    else
    {
        // otherwise, we are in the correct segment, as deep as possible
    }
}