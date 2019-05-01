#pragma once
#include <SFML/Graphics.hpp>

class Car
{
public:
    Car(sf::Vector2f startPos, sf::Vector2f startDir);
    Car(sf::Vector2f startPos, float startAngle);

    struct Controls
    {
        float steer; //should be in [-1;1] range
        float gas;   //should be in [0;1] range
        float brake; //should be in [0;1] range
    };

    void move(Controls, float deltaTime);
    
    sf::Vector2f getPos() const;
    sf::Vector2f getDir() const;

    void draw(sf::RenderWindow&, sf::Vector2f centerPoint);

private:
    sf::Vector2f pos;
    sf::Vector2f dir; 

    sf::RectangleShape sprite;

    float speed;
    float steerSpeed;
};