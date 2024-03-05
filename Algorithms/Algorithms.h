//
// Created by goncalo on 04-03-2024.
//

#ifndef PROJECT_ALGORITHMS_H
#define PROJECT_ALGORITHMS_H


#include "../graph/Graph.h"
#include <queue>
#include <climits>
#include <iostream>
using namespace std;

class Algorithms {
public:
    static bool BFSEdmondsKarp(Graph* g, queue<Vertex*> q);
    static void simpleEdmondsKarp(Graph *g);

    static void citiesWithNotEnoughWater(Graph* g);
};


#endif //PROJECT_ALGORITHMS_H
