//
// Created by goncalo on 04-03-2024.
//

#ifndef PROJECT_ALGORITHMS_H
#define PROJECT_ALGORITHMS_H


#include "../graph/Graph.h"
#include "../Menu.h"
#include <queue>
#include <climits>
#include <iostream>

// Representa quanto uma cidade ganha ou perde água
struct CityWaterLoss{
    std::string cityCode;
    double waterLoss;
};

// Se fossem tirados várias estações ao mesmo tempo, o que aconteceria
struct WaterLossOnStationDelete{
    std::vector<std::string> deletedStationsCodes;
    std::vector<CityWaterLoss> waterLoss;
};

// Representar estatísticas
struct GlobalStatisticsEdges{
    float avg;
    float variance;
    int max_difference;
    int n_edges; // number of edges
};

typedef std::pair<std::string,std::string>PipeStartEndCodes; // Start Code, End Code of Pipe

// Se for removido pipes, qual a water loss
struct WaterLossOnPipeDelete{
    std::vector<PipeStartEndCodes> deletedPipesCodes;
    std::vector<CityWaterLoss> waterLoss;
};

struct WaterLossOnPipeline {
    std::string pipe;
    std::vector<CityWaterLoss> waterLoss;
};



//Note: Functions assume that all necessary "previous functions" have been called

class Algorithms {

public:

public:
    static void calculateWaterInCities(Graph* g);
   //1
    static bool BFSEdmondsKarp(Graph* g, std::queue<Vertex*> q);
    static void simpleEdmondsKarp(Graph *g);
    static void simpleEdmondsKarpThatDoesntDeleteSourceAndSink(Graph *g);
    //2
    static std::vector<City*> CitiesWithNotEnoughWater(Graph* g);
    //3
    // Store on pair (Pipe, float) for each pipe the percentage of decrease/increase that it had after balance
    static void BalanceTheLoad(Graph* g);

    //4
    //returns affected cities

    static void shutDownReservoir(Graph* graph);


    static std::vector<CityWaterLoss> CanShutDownReservoir(Graph* graph, const std::string& reservoirCode);
    static bool DFSShutDownReservoir(Vertex* vertex, std::vector<CityWaterLoss>& cityWaterLoss);
    static std::vector<CityWaterLoss> smartCanShutDownReservoir(Graph* graph, const std::string& reservoirCode);
    static std::vector<CityWaterLoss> CanShutDownReservoirOptimized(Graph* graph, const std::string& reservoirCode);

    //5)
    static void deletePumpingStation(Graph* g);
    //static std::vector<WaterLossOnStationDelete> GetGroupsOfPumpingStationsThatCanBeRemovedSafely(Graph* graph);
   // static std::vector<WaterLossOnStationDelete> GetGroupsOfPumpingStationsThatCanBeRemovedSafelyBruteForce(Graph* graph);
    static std::vector<CityWaterLoss> CanDeletePumpingStationOptimized(Graph* graph, const std::string& stationCode);
    static std::vector<CityWaterLoss> CanDeletePumpingStationFrom0(Graph* graph, const std::string& stationCode);

     //6)
    //static std::vector<WaterLossOnPipeDelete> GetGroupsOfEdgesThatCanBeRemovedSafely(Graph*);
 //   static std::vector<WaterLossOnPipeDelete> GetGroupsOfEdgesThatCanBeRemovedSafelyBruteForce(Graph*);
    static std::vector<WaterLossOnPipeDelete> criticalPipelines(Graph* graph);



    static void AddSourceAndSink(Graph* g);

    static void SetFlowToZero(Graph* graph);
    static void EdmondsKarpThatIgnoresVertex(Graph* graph,Vertex* vertx);
};


#endif //PROJECT_ALGORITHMS_H
