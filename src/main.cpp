#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include "Car.hpp"
#include "Road.hpp"
#include "RayTracer.hpp"
#include "utils.hpp"

void geneticAutoTrainer(sf::RenderWindow&);

int main()
{
    int resX = 1280, resY = 720;
    sf::RenderWindow window(sf::VideoMode(resX, resY), "Auto-Car");
    window.setFramerateLimit(60);
    sf::View view({0, 0}, {1280, -720});
    window.setView(view);

    geneticAutoTrainer(window);



    // Driving test
    Car car({0, 0}, {0, 1});

    Road road(666);
    RayTracer rayTracer(120.f, 8);


    std::chrono::high_resolution_clock clock;

    auto loopStartTime = clock.now();
    float deltaT = 1.f / 60.f;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();

        Car::Controls carControls;
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            carControls.gas = 1;
        else
            carControls.gas = 0;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            carControls.steer = -1;
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            carControls.steer = 1;
        else
            carControls.steer = 0;

        car.move(carControls, deltaT);
        if(!road.moveCarTo(car.getPos()))
        {
            std::cout << "CAR CRASHED!!!!\n";
            std::cout << "Score: " << road.getScore() << '\n';
            return 0;
        }

        rayTracer.doTracing(car.getDir(), road);

        window.clear();
        road.draw(window, car.getPos());
        car.draw(window, car.getPos());
        rayTracer.draw(window, car.getPos(), car.getPos());
        window.display();

        deltaT = std::chrono::duration<float, std::milli>(clock.now() - loopStartTime).count() / 1000.f;
        loopStartTime = clock.now();
        //std::cout << std::fixed << "deltaT: " << deltaT << '\n';
    }

    return 0;
}