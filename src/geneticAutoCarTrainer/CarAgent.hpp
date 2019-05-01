#pragma once
#include "Car.hpp"
#include "Road.hpp"
#include "RayTracer.hpp"
#include "GeneticNet.hpp"

class CarAgent
{
public:
    CarAgent(int raysNumber);
    CarAgent(const CarAgent&, const CarAgent&); //Child of these two

    Car::Controls drive(const std::vector<float>& traceRes);

    void mutate(); //Random mutation

private:
    GeneticNet neuralNet;
};