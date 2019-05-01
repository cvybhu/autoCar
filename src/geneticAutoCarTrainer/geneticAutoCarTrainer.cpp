#include <SFML/Graphics.hpp>
#include "CarAgent.hpp"
#include <chrono>
#include <iostream>

struct AutoCar
{
    AutoCar(int raysNum);
    AutoCar(const AutoCar& b);
    AutoCar(AutoCar&&);

    void reset(int roadSeed);

    CarAgent* agent;
    Car* car;
    Road* road;

    int raysNum;
    bool crashed;

    AutoCar& operator=(AutoCar&& b);
    ~AutoCar(); 
};


const int raysNum = 7;
const int fovAngle = 120.f;
const int populationSize = 50;
std::vector<AutoCar> autoCars(populationSize, raysNum);
RayTracer rayTracer(fovAngle, raysNum);

void doQuickGeneration();

void geneticAutoTrainer(sf::RenderWindow& window)
{
    int showRoadSeed = 1069;

    for(auto& c : autoCars)
        c.reset(showRoadSeed);

    int curGeneration = 0;

//MAIN LOOP
    sf::Vector2f centerPoint;
    std::chrono::high_resolution_clock clock;
    auto loopStartTime = clock.now();
    float deltaT = 1.f / 60.f;
    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Space)
                {
                    doQuickGeneration();
                    showRoadSeed = rand();
                    for(auto& a : autoCars)
                        a.reset(showRoadSeed);
                    curGeneration++;
                    std::cout << "GENERATION: " << curGeneration << '\n';
                    loopStartTime = clock.now();
                }
            }
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();

        for(auto& c : autoCars)
        {
            if(!c.crashed)
            {
                rayTracer.doTracing(c.car->getDir(), *c.road);
                Car::Controls controls = c.agent->drive(rayTracer.get());

                c.car->move(controls, deltaT);
                if(!c.road->moveCarTo(c.car->getPos()))
                    c.crashed = true;
            }
        }

        int curBestCar = 0;
        for(int i = 0; i < populationSize; i++)
        {
            if(autoCars[i].road->getScore() > autoCars[curBestCar].road->getScore()
              && !autoCars[i].crashed)
                curBestCar = i;
        }
        centerPoint = autoCars[curBestCar].car->getPos();

        window.clear();
        autoCars[curBestCar].road->draw(window, centerPoint);

        for(auto& a : autoCars)
        {
            a.car->draw(window, centerPoint);
        }

        rayTracer.doTracing(autoCars[curBestCar].car->getDir(), *autoCars[curBestCar].road);
        rayTracer.draw(window, autoCars[curBestCar].car->getPos(), centerPoint);

        window.display();

        deltaT = std::chrono::duration<float, std::milli>(clock.now() - loopStartTime).count() / 1000.f;
        loopStartTime = clock.now();
    }
}



void doQuickGeneration()
{
    std::cout << "Doing quick gen!\n";

    float generationTime = 10.f;
    float simDeltaT = 1.f/ 60.f;

//Setup new road
    int curRoadSeed = rand();

    for(auto& a : autoCars)
        a.reset(curRoadSeed);

//Run simulation
    for(float t = 0.f; t < generationTime; t += simDeltaT)
    {
        for(auto& a : autoCars)
            if(!a.crashed)
            {
                rayTracer.doTracing(a.car->getDir(), *a.road);
                a.car->move(a.agent->drive(rayTracer.get()), simDeltaT);
                if(!a.road->moveCarTo(a.car->getPos()))
                    a.crashed = true;
            }
    }

//Sort by best score
    std::sort(autoCars.begin(), autoCars.end(), [](const AutoCar& a, const AutoCar& b){
        return a.road->getScore() > b.road->getScore();
    });

//Evolution
    int howManyDied = 0;
    int howManyMutated = 0;
    for(int i = 0; i < populationSize; i++)
    {
        bool dies = (rand() % populationSize < i);

        if(dies)
        {
            howManyDied++;

            delete autoCars[i].agent;

            int parent1 = rand()%i;
            int parent2 = rand()%i;

            autoCars[i].agent = new CarAgent(*autoCars[parent1].agent, *autoCars[parent2].agent);
        }
        else if(rand()%2) 
        { 
            //Mutation
            howManyMutated++;
            autoCars[i].agent->mutate();
        }
    }

    std::cout << howManyDied << " died [*]\n";
    std::cout << howManyMutated << " mutated 0o0o0o\n";
    std::cout << "Done\n";
}



void AutoCar::reset(int roadSeed)
{
    //std::cout << "Resetting with seed: " << roadSeed << '\n';

    if(road)
        delete road;

    road = new Road(roadSeed);

    if(car)
        delete car;
    
    car = new Car({0, 0}, {0, 1});


    crashed = false;
}



AutoCar::AutoCar(int raysNumIn)
{
    raysNum = raysNumIn;

    agent = new CarAgent(raysNum);
    road = nullptr;
    car = nullptr;
    crashed = false;
}

AutoCar::AutoCar(const AutoCar& b)
    : AutoCar(b.raysNum)
{
    if(b.road != nullptr)
        road = new Road(b.road->getSeed());

    if(b.car != nullptr)
        car = new Car({0, 0}, {0, 1});
}

AutoCar::AutoCar(AutoCar&& b)
{
    agent = b.agent;
    b.agent = nullptr;
    car = b.car;
    b.car = nullptr;
    road = b.road;
    b.road = nullptr;
}

AutoCar& AutoCar::operator=(AutoCar&& b)
{
    car = b.car;
    b.car = nullptr;
    road = b.road;
    b.road = nullptr;
    agent = b.agent;
    b.agent = nullptr;

    return *this;
}

AutoCar::~AutoCar()
{
    if(agent)
    {
        delete agent;
        agent = nullptr;
    }

    if(car)
    {
        delete car;
        car = nullptr;
    }

    if(road)
    {
        delete road;
        road = nullptr;
    }
}

