
#include "SFML/Graphics.hpp"
#include <iostream>
#include "../src/new/collision_system.hpp"
#include "../src/new/shape_system.hpp"
#include "../src/new/transform_system.hpp"
#include "../src/new/entity_system.hpp"
#include "../src/new/click_system.hpp"

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
    

    // add entity
    uint64_t entity = entitySystem.createEntity(ecsDb);
    ShapeComponent& shape = entitySystem.addShapeComponent(ecsDb, entity);
    TransformComponent& transform = entitySystem.addTransformComponent(ecsDb, entity);
    ClickableComponent& clickable = entitySystem.addClickableComponent(ecsDb, entity);


    shape.shape = sf::VertexArray(sf::Quads, 4);

    shape.shape[0].position = sf::Vector2f(100.f, 100.f);
    shape.shape[1].position = sf::Vector2f(200.f, 100.f);
    shape.shape[2].position = sf::Vector2f(200.f, 200.f);
    shape.shape[3].position = sf::Vector2f(100.f, 200.f);

    shape.shape[0].color = sf::Color::Blue;
    shape.shape[1].color = sf::Color::Blue;
    shape.shape[2].color = sf::Color::Blue;
    shape.shape[3].color = sf::Color::Blue;

    transform.position = sf::Vector2f(150.f, 150.f);
    transform.dampingFactor = 0.4f;
    transform.velocity = sf::Vector2f(1,1);
    transform.acceleration = 1.f;
   

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

