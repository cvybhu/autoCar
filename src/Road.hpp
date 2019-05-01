#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include <random>

class Road
{
public:
    Road(int seed);

    bool moveCarTo(sf::Vector2f); //Returns false if hits wall
    float getScore() const;

    float rayTrace(sf::Vector2f dir) const;

    void draw(sf::RenderWindow&, sf::Vector2f centerPos) const;

    int getSeed() const;

private:
    std::default_random_engine randEngine;
    std::uniform_real_distribution<float> randDist;
    int seed;

    float getRandFloat();

    struct RoadPoint
    {
        sf::Vector2f pos;
        sf::Vector2f dir;

        sf::Vector2f lSide, rSide;
    };

    std::deque<RoadPoint> points;

    void setSides(RoadPoint&);
    void addNewPoint();

    sf::Vector2f carPos;
    float distanceDriven = 0;

    int lastCarPosIndex = 0;
    int globalCarIndex = 0;

    bool isInFieldOf(int pointIndex, sf::Vector2f pnt);
};
