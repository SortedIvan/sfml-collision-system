
#include "SFML/Graphics.hpp"
#include <iostream>
#include <random>
#include "ecs_systems/collision_system.hpp"
#include "ecs_systems/shape_system.hpp"
#include "ecs_systems/transform_system.hpp"
#include "ecs_systems/entity_system.hpp"
#include "ecs_systems/click_system.hpp"
#include "ecs_systems/follow_system.hpp"
#include "ecs_systems/scatter_system.hpp"

sf::Vector2f randomPointGenerator(sf::Vector2f range);
void randomVelocityGenerator(std::vector<sf::Vector2f>& randomDirections, int scalar, int amountOfRects);
void spawnRandomEntities(int amount, EcsDb& db, EntitySystem& entitySys, sf::Vector2f windowDimensions);
void TryLoadFont(sf::Font& font, std::string path);
sf::Vector2f randomDirVec();
void insertAllRectsIntoQuadTreeTest(std::unique_ptr<QuadNode>& root, EcsDb& db);
void addEntity(EcsDb& db, EntitySystem& entitySys, sf::RenderWindow& window, std::unique_ptr<QuadNode>& root);

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(1000, 800),
        "Shapes");
    window.setFramerateLimit(60); // Set the framerate limit to 60 FPS
    sf::Event e;

    sf::Font font;
    TryLoadFont(font, "testfont.ttf");

    sf::Clock deltaTimeClock;
    sf::Clock fpsClock;
    sf::Time elapsed;
    unsigned int frames = 0;

    sf::Text fpsCounter;
    fpsCounter.setFont(font);
    fpsCounter.setPosition(50.f, 50.f);
    fpsCounter.setCharacterSize(20.f);

    sf::Shader shader;


    EcsDb ecsDb;

    // ========= ECS SYSTEMS ==================
    EntitySystem entitySystem;
    TransformSystem transformSystem;
    ShapeSystem shapeSystem;
    ClickSystem clickSystem;
    FollowSystem followSystem;
    ScatterSystem scatterSystem;
    
    std::unique_ptr<QuadNode> root = std::make_unique<QuadNode>();
    QuadRect boundary;
    boundary.construct(0, 0, 1000, 800);
    root.get()->boundary = boundary;

    spawnRandomEntities(10, ecsDb, entitySystem, (sf::Vector2f)window.getSize());

    insertAllRectsIntoQuadTreeTest(root, ecsDb);

    traverseQuadTree(root, "root");

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
                if (e.mouseButton.button == sf::Mouse::Left) 
                {
                    clickSystem.processClick(ecsDb, (sf::Vector2f)sf::Mouse::getPosition(window));

                    std::cout << "hi";
                }
                else
                {
                    followSystem.activateFollow();
                }
            }

            if (e.type == sf::Event::KeyReleased)
            {
                if (e.key.code == sf::Keyboard::X)
                {
                    scatterSystem.scatterObjects((sf::Vector2f)sf::Mouse::getPosition(window), 50.f, ecsDb);
                }

                if (e.key.code == sf::Keyboard::A)
                {
                    addEntity(ecsDb, entitySystem, window, root);
                }
            }
        }

        // Get delta time and run all updates
        // this can also be done differently, previous_captured_time - current_time
        sf::Time deltaTime = deltaTimeClock.restart();
        
        // ========== Update ==============
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        transformSystem.moveAllComponents(ecsDb, deltaTime.asSeconds(), 1000, 800, root);
        shapeSystem.moveShapesIfNeeded(ecsDb);
        
        followSystem.setFollowTarget(ecsDb, (sf::Vector2f)mousePos);


        // Clear the window
        window.clear(sf::Color::Black);
        
        // draw
        visualizeTree(root, window);
        shapeSystem.drawShapes(ecsDb, window);
        window.draw(fpsCounter);


        // display
        window.display();

        // Calculate FPS
        frames++;
        elapsed += fpsClock.restart();
        if (elapsed >= sf::seconds(1.0f)) 
        {
            fpsCounter.setString("FPS: " + std::to_string(frames));
            frames = 0;
            elapsed = sf::Time::Zero;
        }
    }

    return 0;
}

void insertAllRectsIntoQuadTreeTest(std::unique_ptr<QuadNode>& root, EcsDb& db)
{
    for (int i = 0; i < db.transformComponents.size(); i++)
    {
        QuadLeaf leaf;
        leaf.position = db.transformComponents[i].position;
        leaf.transformId = db.transformComponents[i].transform_id;

        root.get()->insert(leaf);
    }
}

void TryLoadFont(sf::Font& font, std::string path)
{
    if (!font.loadFromFile(path))
    {
        std::cout << "Error loading the font file" << std::endl;
        system("pause");
    }
}

sf::Vector2f randomDirVec()
{
    const double PI = 3.1415926;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 360);

    return sf::Vector2f(std::cos(dist6(rng)) * (PI / 180), std::sin(dist6(rng)) * (PI / 180));
}

sf::Vector2f randomPointGenerator(sf::Vector2f range)
{
    std::random_device dev;
    std::mt19937 rng(dev());

    std::uniform_int_distribution<std::mt19937::result_type> randX(0, range.x);
    std::uniform_int_distribution<std::mt19937::result_type> randY(0, range.y);

    return sf::Vector2f(randX(rng), randY(rng));
}

void addEntity(EcsDb& db, EntitySystem& entitySys, sf::RenderWindow& window, std::unique_ptr<QuadNode>& root)
{
    int widthRect = 15.f;
    int heightRect = 15.f;
    sf::Vector2f positionClicked = (sf::Vector2f)sf::Mouse::getPosition(window);

    uint64_t entity = entitySys.createEntity(db);

    ClickableComponent& click = entitySys.addClickableComponent(db, entity);
    ShapeComponent& shape = entitySys.addShapeComponent(db, entity);
    TransformComponent& transform = entitySys.addTransformComponent(db, entity);
    entitySys.addFollowMouseComponent(db, entity);

    shape.shape = sf::VertexArray(sf::Quads, 4);

    shape.shape[0].position = sf::Vector2f(positionClicked.x - widthRect / 2, positionClicked.y - heightRect / 2);
    shape.shape[1].position = sf::Vector2f(positionClicked.x + widthRect / 2, positionClicked.y - heightRect / 2);
    shape.shape[2].position = sf::Vector2f(positionClicked.x + widthRect / 2, positionClicked.y + heightRect / 2);
    shape.shape[3].position = sf::Vector2f(positionClicked.x - widthRect / 2, positionClicked.y + heightRect / 2);

    shape.shape[0].color = sf::Color::Blue;
    shape.shape[1].color = sf::Color::Blue;
    shape.shape[2].color = sf::Color::Blue;
    shape.shape[3].color = sf::Color::Blue;

    transform.position = positionClicked;
    transform.dampingFactor = 0.9f;
    //transform.velocity = randomDirVec() * 20000.f;
    transform.velocity = sf::Vector2f(0, 0);
    transform.acceleration = sf::Vector2f(3, 3);
    transform.isMoving = true;

    QuadLeaf leaf; 
    leaf.position = transform.position;
    leaf.transformId = transform.transform_id;

    root.get()->insert(leaf);
}

void spawnRandomEntities(int amount, EcsDb& db, EntitySystem& entitySys, sf::Vector2f windowDimensions)
{

    int widthRect = 15.f;
    int heightRect = 15.f;

    //std::vector<sf::Vector2f> randomDirections;
    //randomVelocityGenerator(randomDirections, 5.f, amount);

    for (int i = 0; i < amount; i++)
    {
        uint64_t entity = entitySys.createEntity(db);

        ClickableComponent& click = entitySys.addClickableComponent(db, entity);
        ShapeComponent& shape = entitySys.addShapeComponent(db, entity);
        TransformComponent& transform = entitySys.addTransformComponent(db, entity);
        entitySys.addFollowMouseComponent(db, entity);

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
        transform.dampingFactor = 0.9f;
        //transform.velocity = randomDirVec() * 20000.f;
        transform.velocity = sf::Vector2f(0,0);
        transform.acceleration = sf::Vector2f(3, 3);
        transform.isMoving = true;
    }
}