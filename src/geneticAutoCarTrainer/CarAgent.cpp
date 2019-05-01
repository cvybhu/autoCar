#include "CarAgent.hpp"

CarAgent::CarAgent(int raysNumber)
    : neuralNet(raysNumber, 10, 3)
{}

CarAgent::CarAgent(const CarAgent& a, const CarAgent& b) //Child of these two
    : neuralNet(a.neuralNet, b.neuralNet)
{}

void CarAgent::mutate()
{
    neuralNet.mutate();
}

Car::Controls CarAgent::drive(const std::vector<float>& rayTraceRes)
{
    Car::Controls res;

    auto&& netRes = neuralNet.feed(rayTraceRes);

    res.gas = netRes[0];
    res.steer = -netRes[1] + netRes[2];

    return res;
}