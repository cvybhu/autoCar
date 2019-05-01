#include "utils.hpp"
#include <cmath>
#include <tuple>

sf::Vector2f vecFromAngle(float angle)
{
    return {(float)sin(angle), (float)cos(angle)};
}

float angleFromVec(sf::Vector2f vec)
{
    if(vec == sf::Vector2f(0, 0))
        return -1;

    float arcsin = asin(vec.y / length(vec));

    if(vec.x > 0)
        return M_PI / 2 - arcsin;
    else
        return 1.5f * M_PI + arcsin;
}

float length(sf::Vector2f vec)
{
    return sqrt(vec.x*vec.x + vec.y*vec.y);
}

sf::Vector2f normalize(sf::Vector2f vec)
{
    if(vec == sf::Vector2f(0, 0))
        return vec;

    return vec / length(vec);
}

sf::Vector2f perpendicular(sf::Vector2f vec)
{
    return {vec.y, -vec.x};
}

float dot(sf::Vector2f v1, sf::Vector2f v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

float cross(sf::Vector2f v1, sf::Vector2f v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

static bool isOnSegment(std::pair<sf::Vector2f, sf::Vector2f> segment, sf::Vector2f pnt);

sf::Vector2f lineIntersectPoint(std::pair<sf::Vector2f, sf::Vector2f> line1,
                                std::pair<sf::Vector2f, sf::Vector2f> line2)
{
    assert(line1.first != line1.second);
    assert(line2.first != line2.second);

    //Equation taken from wikipedia
    float denominator = cross(line1.first - line1.second, line2.first - line2.second);

    if(denominator == 0) // parallel
    {
        if(isOnSegment(line2, line1.first))
            return line1.first;

        if(isOnSegment(line2, line1.second))
            return line1.second;

        if(isOnSegment(line1, line2.first))
            return line2.first;

        return line2.second;
    }

    float cross1 = cross(line1.first, line1.second);
    float cross2 = cross(line2.first, line2.second);

    sf::Vector2f res;
    res.x =   cross1 * (line2.first.x - line2.second.x)
            - cross2 * (line1.first.x - line1.second.x);
    res.x /= denominator;

    res.y =   cross1 * (line2.first.y - line2.second.y)
            - cross2 * (line1.first.y - line1.second.y);
    res.y /= denominator;

    return res;
}

std::optional<sf::Vector2f> segmentIntersectPoint(std::pair<sf::Vector2f, sf::Vector2f> segment1,
                                                  std::pair<sf::Vector2f, sf::Vector2f> segment2)
{
    auto interPoint = lineIntersectPoint(segment1, segment2);

    if(isOnSegment(segment1, interPoint) && isOnSegment(segment2, interPoint))
        return interPoint;
    else
        return {};
}

static bool isOnSegment(std::pair<sf::Vector2f, sf::Vector2f> segment, sf::Vector2f pnt)
{
    float maxX, maxY, minX, minY;

    if(segment.first.x > segment.second.x)
    {
        maxX = segment.first.x;
        minX = segment.second.x;
    }
    else
    {
        maxX = segment.second.x;
        minX = segment.first.x;
    }

    if(segment.first.y > segment.second.y)
    {
        maxY = segment.first.y;
        minY = segment.second.y;
    }
    else
    {
        maxY = segment.second.y;
        minY = segment.first.y;
    }

    return (minX <= pnt.x && pnt.x <= maxX && minY <= pnt.y && pnt.y <= maxY);
}


bool isInTriangle(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c,  sf::Vector2f pnt)
{
    assert(a != b && a != c && b != c);

    float cross1 = cross(b - a, b - pnt);
    float cross2 = cross(c - b, c - pnt);
    float cross3 = cross(a - c, a - pnt);

    return (cross1 * cross2 >= 0 && cross1 * cross3 >= 0 && cross2 * cross3 >= 0);
}