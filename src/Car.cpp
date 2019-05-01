#include "Car.hpp"
#include "utils.hpp"
#include <cmath>
#include <iostream>

Car::Car(sf::Vector2f startPos, sf::Vector2f startDir)
{
    pos = startPos;
    dir = startDir;

    sprite = sf::RectangleShape(sf::Vector2f(25, 50));
    sprite.setFillColor(sf::Color::Blue);
    sprite.setOrigin(sf::Vector2f(sprite.getSize().x / 2, sprite.getSize().y / 2)); 

    speed = 1000.f;
    steerSpeed = 3.f;
}

Car::Car(sf::Vector2f startPos, float angle)
    : Car(startPos, vecFromAngle(angle))
{}

void Car::draw(sf::RenderWindow& window, sf::Vector2f centerPoint)
{
    sprite.setPosition(pos - centerPoint);
    sprite.setRotation(-angleFromVec(dir) * 360.f  / 2.f / M_PI);
    window.draw(sprite);
}

void Car::move(Controls controls, float deltaT)
{
    if(controls.gas > 1)
        controls.gas = 1;

    if(controls.gas < 0)
        controls.gas = 0;

    if(controls.steer < -1)
        controls.steer = -1;

    if(controls.steer > 1)
        controls.steer = 1;

    pos += dir * controls.gas * speed * deltaT;

    if(controls.gas != 0)
        dir = vecFromAngle(angleFromVec(dir) + controls.steer * steerSpeed * deltaT);
}

sf::Vector2f Car::getPos() const
{
    return pos;
}

sf::Vector2f Car::getDir() const
{
    return dir;
}