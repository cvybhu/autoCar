#pragma once
#include "Road.hpp"

class RayTracer
{
public:
    RayTracer(float fovAngleInDegrees, int raysNum);

    void doTracing(sf::Vector2f dir, const Road& road);

    const std::vector<float>& get() const;

    void draw(sf::RenderWindow&, sf::Vector2f pos, sf::Vector2f centerPoint);

private:
    std::vector<float> traceRes;
    float fovAngle;
    sf::Vector2f dir;
};