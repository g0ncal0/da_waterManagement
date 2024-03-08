//
// Created by goncalo on 04-03-2024.
//

#ifndef PROJECT_ALGORITHMS_H
#define PROJECT_ALGORITHMS_H


#include "../graph/Graph.h"
#include <queue>
#include <climits>
#include <iostream>

struct CityWaterLoss{
    std::string cityCode;
    double waterLoss;
};

struct WaterLossOnStationDelete{
std::vector<std::string> deletedStationsCodes;
std::vector<CityWaterLoss> waterLoss;
};
typedef std::pair<std::string,std::string>PipeStartEndCodes;
struct WaterLossOnPipeDelete{
    std::vector<PipeStartEndCodes> deletedPipesCodes;
    std::vector<CityWaterLoss> waterLoss;
};

//Note: Functions assume that all necessary "previous functions" have been called
class Algorithms {
public:
   //1
    static bool BFSEdmondsKarp(Graph* g, std::queue<Vertex*> q);
    static void simpleEdmondsKarp(Graph *g);
    //2
    static std::vector<City*> CitiesWithNotEnoughWater(Graph* g);
    //3
    // Store on pair (Pipe, float) for each pipe the percentage of decrease/increase that it had after balance
    static std::vector<std::pair<Edge*, float>> BalanceTheLoad(Graph* g);

    //4
    //returns affected cities
    static std::vector<CityWaterLoss> CanShutDownReservoir(Graph* graph, const std::string& reservoirCode);
    static bool DFSShutDownReservoir(Vertex* vertex, std::vector<CityWaterLoss>& cityWaterLoss);
    static std::vector<CityWaterLoss> smartCanShutDownReservoir(Graph* graph, const std::string& reservoirCode);
    static std::vector<CityWaterLoss> CanShutDownReservoirs(Graph* graph, const std::vector<std::string>& reservoirCode);

    //5)
    //static std::vector<WaterLossOnStationDelete> GetGroupsOfPumpingStationsThatCanBeRemovedSafely(Graph* graph);
    static std::vector<WaterLossOnStationDelete> GetGroupsOfPumpingStationsThatCanBeRemovedSafelyBruteForce(Graph* graph);

     //6)
    //static std::vector<WaterLossOnPipeDelete> GetGroupsOfEdgesThatCanBeRemovedSafely(Graph*);
    static std::vector<WaterLossOnPipeDelete> GetGroupsOfEdgesThatCanBeRemovedSafelyBruteForce(Graph*);

};


#endif //PROJECT_ALGORITHMS_H
