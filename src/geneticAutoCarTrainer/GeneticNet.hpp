#pragma once
#include <vector>

class GeneticNet
{
public:
    GeneticNet(int inNodes, int hiddenNodes, int outNodes);
    GeneticNet(const GeneticNet&, const GeneticNet&); //child of these two

    const std::vector<float>& feed(const std::vector<float>&);

    void mutate(); //Random mutation

private:
    int inNum, hiddenNum, outNum;

    std::vector< std::vector<float> > weights[2];
    
    std::vector<float> hiddenVals;
    std::vector<float> outVals;
};