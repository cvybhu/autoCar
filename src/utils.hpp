#pragma once
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <optional>
#include <cassert>

float angleFromVec(sf::Vector2f);
sf::Vector2f vecFromAngle(float);

template <class T>
std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& vec)
{
    return os << "(" << vec.x << "," << vec.y << ")";
}

sf::Vector2f normalize(sf::Vector2f);
sf::Vector2f perpendicular(sf::Vector2f);

float length(sf::Vector2f);
float dot(sf::Vector2f v1, sf::Vector2f v2);
float cross(sf::Vector2f v1, sf::Vector2f v2);

sf::Vector2f lineIntersectPoint(std::pair<sf::Vector2f, sf::Vector2f> line1,
                                std::pair<sf::Vector2f, sf::Vector2f> line2);

std::optional<sf::Vector2f> segmentIntersectPoint(std::pair<sf::Vector2f, sf::Vector2f> segment1,
                                                  std::pair<sf::Vector2f, sf::Vector2f> segment2);

bool isInTriangle(sf::Vector2f, sf::Vector2f, sf::Vector2f,  sf::Vector2f pnt);