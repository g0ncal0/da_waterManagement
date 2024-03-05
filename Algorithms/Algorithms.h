//
// Created by goncalo on 04-03-2024.
//

#ifndef PROJECT_ALGORITHMS_H
#define PROJECT_ALGORITHMS_H


#include "../graph/Graph.h"
#include <queue>
#include <climits>
#include <iostream>


class Algorithms {
public:
    static bool BFSEdmondsKarp(Graph* g, std::queue<Vertex*> q);
    static void simpleEdmondsKarp(Graph *g);

    static std::vector<City*> citiesWithNotEnoughWater(Graph* g);

    // Store on pair (Pipe, float) for each pipe the percentage of decrease/increase that it had after balance
    static std::vector<std::pain<Edge*, float>> BalanceTheLoad(Graph* g);

    static void

};


#endif //PROJECT_ALGORITHMS_H
