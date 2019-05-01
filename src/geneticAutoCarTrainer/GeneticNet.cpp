#include "GeneticNet.hpp"
#include <cassert>
#include <cmath>

GeneticNet::GeneticNet(int inNodes, int hiddenNodes, int outNodes)
    : inNum(inNodes), hiddenNum(hiddenNodes), outNum(outNodes)
{
    hiddenVals.resize(hiddenNum);
    outVals.resize(outNum);

    weights[0].resize(inNodes, std::vector<float>(hiddenNodes));
    weights[1].resize(hiddenNodes, std::vector<float>(outNum));

    for(int i = 0; i < 2; i++)
    for(auto& vec : weights[i])
    for(float& w : vec)
    {
        int randRange = 100*1000;
        w = (float)(rand() % (2*randRange+1) - randRange) / (float)randRange;
    }
}

GeneticNet::GeneticNet(const GeneticNet& a, const GeneticNet& b)
    : GeneticNet(a.inNum, a.hiddenNum, a.outNum)
{
    assert(a.inNum == b.inNum && a.hiddenNum == b.hiddenNum && a.outNum == b.outNum);

// Do cross-over
    for(int i = 0; i < inNum; i++)
    for(int h = 0; h < hiddenNum; h++)
        weights[0][i][h] = (rand()%2 ? a.weights[0][i][h] : b.weights[0][i][h]);

    for(int h = 0; h < hiddenNum; h++)
    for(int o = 0; o < outNum; o++)
        weights[1][h][o] = (rand()%2 ? a.weights[1][h][o] : b.weights[1][h][o]);
}

void GeneticNet::mutate()
{
    int howManyMutate = (1+ rand() % (inNum * hiddenNum + hiddenNum * outNum)) / 10;
    //max 1/10th mutates

    for(int m = 0; m < howManyMutate; m++)
    {
        float* curWeight;

        if(rand()%2)
            curWeight = &weights[0][rand()%inNum][rand()%hiddenNum];
        else
            curWeight = &weights[1][rand()%hiddenNum][rand()%outNum];

        *curWeight = (rand()%2 ? *curWeight * 2.f : -*curWeight);
    }
}


static float sigmoid(float x)
    {return 1.f / (1.f + exp(-x));}

const std::vector<float>& GeneticNet::feed(const std::vector<float>& in)
{
    assert((int)in.size() == inNum);

    for(int h = 0; h < hiddenNum; h++)
    {
        hiddenVals[h] = 0;

        for(int i = 0; i < inNum; i++)
            hiddenVals[h] += weights[0][i][h] * in[i];

        hiddenVals[h] = sigmoid(hiddenVals[h]);
    }

    for(int o = 0; o < outNum; o++)
    {
        outVals[o] = 0;

        for(int h = 0; h < hiddenNum; h++)
            outVals[o] += weights[1][h][o] * hiddenVals[h];

        outVals[o] = sigmoid(outVals[o]);
    }

    return outVals;
}