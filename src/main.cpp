
#include "SFML/Graphics.hpp"
#include <iostream>
#include <random>
#include "../src/new/collision_system.hpp"
#include "../src/new/shape_system.hpp"
#include "../src/new/transform_system.hpp"
#include "../src/new/entity_system.hpp"
#include "../src/new/click_system.hpp"

sf::Vector2f randomPointGenerator(sf::Vector2f range);
void spawnRandomEntities(int amount, EcsDb& db, EntitySystem& entitySys, sf::Vector2f windowDimensions);


int main()
{

    sf::RenderWindow window(
        sf::VideoMode(1000, 800),
        "Shapes");

    window.setFramerateLimit(60); // Set the framerate limit to 60 FPS

    sf::Event e;

    sf::Clock deltaTimeClock;

    EcsDb ecsDb;

    // ========= ECS SYSTEMS ==================
    EntitySystem entitySystem;
    TransformSystem transformSystem;
    ShapeSystem shapeSystem;
    ClickSystem clickSystem;
    
    spawnRandomEntities(30, ecsDb, entitySystem, (sf::Vector2f)window.getSize());
   
    // Main loop
    while (window.isOpen())
    {

        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }

            if (e.type == sf::Event::MouseButtonPressed)
            {
                clickSystem.processClick(ecsDb, (sf::Vector2f)sf::Mouse::getPosition(window));
                std::cout << "hi";
            }
        }

        // Get delta time and run all updates
        // this can also be done differently, previous_captured_time - current_time
        sf::Time deltaTime = deltaTimeClock.restart();
        
        // ========== Update ==============
        transformSystem.moveAllComponents(ecsDb, deltaTime.asSeconds());
        shapeSystem.moveShapesIfNeeded(ecsDb);

        // Clear the window
        window.clear(sf::Color::Black);
        
        // draw
        shapeSystem.drawShapes(ecsDb, window);

        // display
        window.display();
    }

    return 0;
}

sf::Vector2f randomPointGenerator(sf::Vector2f range)
{
    std::random_device dev;
    std::mt19937 rng(dev());

    std::uniform_int_distribution<std::mt19937::result_type> randX(0, range.x);
    std::uniform_int_distribution<std::mt19937::result_type> randY(0, range.y);

    return sf::Vector2f(randX(rng), randY(rng));
}

void spawnRandomEntities(int amount, EcsDb& db, EntitySystem& entitySys, sf::Vector2f windowDimensions)
{

    int widthRect = 100.f;
    int heightRect = 100.f;

    for (int i = 0; i < amount; i++)
    {
        uint64_t entity = entitySys.createEntity(db);

        ClickableComponent& click = entitySys.addClickableComponent(db, entity);
        ShapeComponent& shape = entitySys.addShapeComponent(db, entity);
        TransformComponent& transform = entitySys.addTransformComponent(db, entity);
        
        shape.shape = sf::VertexArray(sf::Quads, 4);

        // Generate a random point within the screen
        sf::Vector2f point = randomPointGenerator(windowDimensions);

        shape.shape[0].position = sf::Vector2f(point.x - widthRect / 2, point.y - heightRect / 2);
        shape.shape[1].position = sf::Vector2f(point.x + widthRect / 2, point.y - heightRect / 2);
        shape.shape[2].position = sf::Vector2f(point.x + widthRect / 2, point.y + heightRect / 2);
        shape.shape[3].position = sf::Vector2f(point.x - widthRect / 2, point.y + heightRect / 2);

        shape.shape[0].color = sf::Color::Blue;
        shape.shape[1].color = sf::Color::Blue;
        shape.shape[2].color = sf::Color::Blue;
        shape.shape[3].color = sf::Color::Blue;

        transform.position = point;
        transform.dampingFactor = 0.02f;
        transform.velocity = sf::Vector2f(1, 1);
        transform.acceleration = 1.f;
    }
}

