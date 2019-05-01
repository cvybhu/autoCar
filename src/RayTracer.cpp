#include "RayTracer.hpp"
#include "utils.hpp"
#include <cmath>

RayTracer::RayTracer(float fovAngleInDregrees, int raysNum)
{
    fovAngle = fovAngleInDregrees / 360.f * 2.f * M_PI;
    traceRes.resize(raysNum);
}

void RayTracer::doTracing(sf::Vector2f dirIn, const Road& road)
{
    dir = dirIn;

    float startAngle = angleFromVec(dir) - fovAngle/2;
    float angleDelta = fovAngle / ((float)traceRes.size() - 1.f);

    for(int r = 0; r < (int)traceRes.size(); r++)
    {
        auto curDir = vecFromAngle(startAngle + angleDelta * r);

        traceRes[r] = road.rayTrace(curDir);
    }
}

const std::vector<float>& RayTracer::get() const
{
    return traceRes;
}

void RayTracer::draw(sf::RenderWindow& window, sf::Vector2f pos, sf::Vector2f centerPoint)
{
    float startAngle = angleFromVec(dir) - fovAngle/2;
    float angleDelta = fovAngle / ((float)traceRes.size() - 1.f);

    sf::VertexArray rays(sf::Lines, 2 * traceRes.size());
    sf::Color rayColor = sf::Color::Green;

    for(int r = 0; r < (int)traceRes.size(); r++)
    {
        auto curDir = vecFromAngle(startAngle + angleDelta * r);

        rays[2*r].position = pos - centerPoint;
        rays[2*r].color = rayColor;

        rays[2*r+1].position = pos - centerPoint + curDir * traceRes[r];
        rays[2*r+1].color = rayColor;
    }

    window.draw(rays);
}