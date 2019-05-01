#include "Road.hpp"
#include "utils.hpp"
#include <cmath>

Road::Road(int seedIn)
    : randEngine(seedIn),
      randDist(-1, 1)
{
    seed = seedIn;

    points.clear();

    points.emplace_back();
    points[0].pos = {0, -10};
    points[0].dir = {0.0000001, 1};
    setSides(points[0]);

    for(int i = 0; i < 50; i++)
        addNewPoint();

    carPos = {0, 0};
}

float Road::getRandFloat() //Returns float in [-1;1] range
    {return randDist(randEngine);}

void Road::setSides(RoadPoint& pnt)
{
    float roadWidth = 40.f;

    auto perpVec = perpendicular(pnt.dir);
    pnt.lSide = pnt.pos - perpVec * roadWidth;
    pnt.rSide = pnt.pos + perpVec * roadWidth;
}

void Road::addNewPoint()
{
    float maxAngleChange = 50.f * M_PI/360.f;
    float angleCoeff = getRandFloat();
    float angleChange = maxAngleChange * angleCoeff;

    points.emplace_back();
    RoadPoint& curPoint = points.back();

    curPoint.dir = vecFromAngle(angleFromVec(points[points.size()-2].dir) + angleChange);
    curPoint.pos = points[points.size()-2].pos + curPoint.dir * 50.f;
    setSides(curPoint);

    if(points.size() > 2)
    {
        points[points.size()-2].dir = normalize(points.back().pos - points[points.size()-3].pos);
        setSides(points[points.size()-2]);
    }
}

bool Road::moveCarTo(sf::Vector2f newPos)
{
    int whereIsIndex = -1;

    for(int p = 0; p+1 < (int)points.size(); p++)
        if(isInFieldOf(p, newPos))
        {
            whereIsIndex = p;
            break;
        }

    if(whereIsIndex == -1)
        return false;

    carPos = newPos;

    int howManyBehind = 20;
    
    globalCarIndex += (whereIsIndex - lastCarPosIndex);

    auto betweenVec = points[whereIsIndex+1].pos - points[whereIsIndex].pos;
    auto carPosVec = carPos - points[whereIsIndex].pos;
    auto project = dot(carPosVec, betweenVec) / dot(betweenVec, betweenVec) * betweenVec;
    float betweenPointsDist = std::max(length(project) / length(betweenVec), 0.f);//betweenPointsDist);

    lastCarPosIndex = whereIsIndex;

    distanceDriven = std::max(distanceDriven, (float)globalCarIndex + betweenPointsDist);

    for(int i = 0; i + howManyBehind < whereIsIndex; i++)
    {
        points.pop_front();
        addNewPoint();
        lastCarPosIndex--;
    }

    return true;
}

bool Road::isInFieldOf(int pointIndex, sf::Vector2f pnt)
{
    RoadPoint& cur = points[pointIndex];
    RoadPoint& nxt = points[pointIndex+1];

    return isInTriangle(cur.lSide, cur.rSide, nxt.lSide, pnt) 
        || isInTriangle(nxt.lSide, nxt.rSide, cur.rSide, pnt);
}

float Road::getScore() const
{
    return distanceDriven;
}

static std::pair<sf::Vector2f, sf::Vector2f> scale(std::pair<sf::Vector2f, sf::Vector2f> segment, float scalar);

float Road::rayTrace(sf::Vector2f traceDir) const
{
    float maxTraceDist = 10000.f;

    auto farPoint = carPos + normalize(traceDir) * maxTraceDist;

    float res = maxTraceDist;

    auto checkIntersectWith = [this, &farPoint, &res](std::pair<sf::Vector2f, sf::Vector2f> segment)
    {
         auto intersectPoint = segmentIntersectPoint(
            {carPos, farPoint},
            segment
        );

        if(intersectPoint.has_value())
            res = std::min(res, length(carPos - *intersectPoint));
    };

    for(int p = 0; p+1 < (int)points.size(); p++)
    {
        checkIntersectWith(scale({points[p].lSide, points[p+1].lSide}, 1.2));
        checkIntersectWith(scale({points[p].rSide, points[p+1].rSide}, 1.2));
    }

    checkIntersectWith({points[0].lSide, points[0].rSide});

    return res;
}

static std::pair<sf::Vector2f, sf::Vector2f> scale(std::pair<sf::Vector2f, sf::Vector2f> segment, float scalar)
{
    auto mid = (segment.first + segment.second) / 2.f;

    return {mid + (segment.first- mid) * scalar, mid + (segment.second - mid) * scalar};
}


void Road::draw(sf::RenderWindow& window, sf::Vector2f centerPoint) const
{
    bool debug = 0;

    if(!debug)
    {
        sf::Color roadColor(124, 113, 113);

        sf::VertexArray roadVerts(sf::TrianglesStrip, 2*points.size());

        for(int p = 0; p < (int)points.size(); p++)
        {
            roadVerts[2*p].position = points[p].lSide - centerPoint;
            roadVerts[2*p+1].position = points[p].rSide - centerPoint;
            roadVerts[2*p].color = roadColor;
            roadVerts[2*p+1].color = roadColor;
        }

        window.draw(roadVerts);
    }
    else
    {
        sf::VertexArray middleLine(sf::LinesStrip, points.size());
        for(int i = 0; i < (int)points.size(); i++)
        {
            middleLine[i].position = points[i].pos - centerPoint;
            middleLine[i].color = sf::Color::Red;
        }

        window.draw(middleLine);

        sf::VertexArray perpLines(sf::Lines, 2*points.size());
        for(int i = 0; i < (int)points.size(); i++)
        {
            perpLines[2*i].position = points[i].lSide - centerPoint;
            perpLines[2*i].color = sf::Color::Red;
            perpLines[2*i+1].position = points[i].rSide - centerPoint;
            perpLines[2*i+1].color = sf::Color::Red;
        }

        window.draw(perpLines);

        sf::VertexArray lSide(sf::LinesStrip, points.size());
        for(int i = 0; i < (int)points.size(); i++)
        {
            lSide[i].position = points[i].lSide - centerPoint;
            lSide[i].color = sf::Color::Red;
        }

        window.draw(lSide);

        sf::VertexArray rSide(sf::LineStrip, points.size());
        for(int i = 0; i < (int)points.size(); i++)
        {
            rSide[i].position = points[i].rSide - centerPoint;
            rSide[i].color = sf::Color::Red;
        }

        window.draw(rSide);

        sf::VertexArray whereIsTriangle(sf::Triangles, 3);
        for(int i = 0; i+1 < (int)points.size(); i++)
        {
            auto&& cur = points[i];
            auto&& nxt = points[i+1];

            bool found = false;

            if(isInTriangle(cur.lSide, cur.rSide, nxt.lSide,  carPos))
            {
                whereIsTriangle[0].position = cur.lSide - centerPoint;
                whereIsTriangle[1].position = cur.rSide - centerPoint;
                whereIsTriangle[2].position = nxt.lSide - centerPoint;
                found = true;
            }

            if(isInTriangle(nxt.lSide, nxt.rSide, cur.rSide,  carPos))
            {
                whereIsTriangle[0].position = nxt.lSide - centerPoint;
                whereIsTriangle[1].position = nxt.rSide - centerPoint;
                whereIsTriangle[2].position = cur.rSide - centerPoint;
                found = true;
            }

            if(found)
            {
                whereIsTriangle[0].color = sf::Color::Red;
                whereIsTriangle[1].color = sf::Color::Red;
                whereIsTriangle[2].color = sf::Color::Red;

                window.draw(whereIsTriangle);
            }
        }
    }
}

int Road::getSeed() const
{
    return seed;
}
