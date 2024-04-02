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
    int waterLoss;
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
    /**
     * A simple algorithm to calculate the water that enters every city O(n). Changes the totalwaterin var in the cities. O(N)
     * @param g graph
     */
    static void calculateWaterInCities(Graph* g);
   //1 ------------------------------------------------------------
    static bool BFSEdmondsKarp(Graph* g, std::queue<Vertex*> q);

    /**
     * Normal BFS Edmonds Karp
     * @param g
     * @param q
     * @return
     */
    static void simpleEdmondsKarp(Graph *g);
    static void simpleEdmondsKarpThatDoesntDeleteSourceAndSink(Graph *g);
    //2 ------------------------------------------------------------

    /**
     * Traverses all cities, checking if amount of water reached is enough. O(n), where n is the number of vertixes of graph
     * @param graph
     * @return vector of cities that do not have enough water
     */
    static std::vector<City*> CitiesWithNotEnoughWater(Graph* g);
    //3 ------------------------------------------------------------

    static void BalanceTheLoad(Graph* g);
    static bool auxBFSBalanceTheLoad(Graph* g, std::queue<Vertex*> q, const std::string& source, const std::string& sink, int& maxFlow);

    //4 ------------------------------------------------------------
    /*** @brief This function calculates the change in water reaching the cities when a reservoir is removed.
     * Output: Cities that have lost water are printed, alongside the amount of water in cubic meters per second
     * Complexity: O(V * E^2), because, even though it runs the optimized algorithm, it still calls the Edmonds-Karp algorithm before, to obtain the "default" flows arriving at the cities
     * @param graph The graph on which to run this operation.
    */
    static void shutDownReservoir(Graph* graph);

    /** @brief This function calculates the change in water reaching the cities when a reservoir is removed. It is not optimized, as it will run the Edmonds-Karp algorithm from the beginning. \n
    * Output: If no errors occur, nothing will be printed \n
    * Complexity: O(V * E^2)
    * @param graph The graph on which to run this operation
    * @param reservoirCode The reservoir which we are attempting to remove
    */
    static std::vector<CityWaterLoss> CanShutDownReservoir(Graph* graph, const std::string& reservoirCode);
    /** @brief This function calculates the change in water reaching the cities when a reservoir is removed. It is optimized, so that the Edmonds-Karp does not need to be run in its entirety. However, the standard incoming flows of cities must already be set at the beginning of the algorithm. It is assumed that the super-source and super-sink exist in the graph. Due to this precondition, the wrapper function void shutDownReservoir(Graph* graph) was created. \n
    * Output:  If no errors occur, nothing will be printed \n
    * Complexity: In the worst case, it could be the same as running the Edmonds-Karp algorithm from zero (O(V * E^2)), but, in a lot of cases, a solution can be reached with a small number of iterations
    * @param graph The graph on which to run this operation
    * @param reservoirCode The reservoir which we are attempting to remove
    */
    static std::vector<CityWaterLoss> CanShutDownReservoirOptimized(Graph* graph, const std::string& reservoirCode);

    //5) ------------------------------------------------------------
    /** @brief This function calculates the change in water reaching the cities when a pumping station is removed.
    * Output: Cities that have lost water are printed, alongside the amount of water in cubic meters per second
    * Complexity: O(V * E^2), because, even though it runs the optimized algorithm, it still calls the Edmonds-Karp algorithm before, to obtain the "default" flows arriving at the cities
    * @param graph The graph on which to run this operation.
    */
    static void deletePumpingStation(Graph* g);
    /** @brief This function calculates the change in water reaching the cities when a pumping station is removed. It is optimized, so that the Edmonds-Karp does not need to be run in its entirety. However, the standard incoming flows of cities must already be set at the beginning of the algorithm. It is assumed that the super-source and super-sink exist in the graph. Due to this precondition, the wrapper function void shutDownReservoir(Graph* graph) was created. \n
    * Output: If successful, nothing is printed
    * Complexity: In the worst case, it could be the same as running the Edmonds-Karp algorithm from zero (O(V * E^2)), but, in a lot of cases, a solution can be reached with a small number of iterations
    * @param graph The graph on which to run this operation.
    * @param stationCode The pumping station to remove from the graph
    */
    static std::vector<CityWaterLoss> CanDeletePumpingStationOptimized(Graph* graph, const std::string& stationCode);
    /** @brief This function calculates the change in water reaching the cities when a pumping station is removed. It runs the Edmonds-Karp algorithm from the beginning.
    * Output: If successful, nothing is printed
    * Complexity: O(V * E^2)
    * @param graph The graph on which to run this operation.
    * @param stationCode The pumping station to remove from the graph
    */
    static std::vector<CityWaterLoss> CanDeletePumpingStationFrom0(Graph* graph, const std::string& stationCode);

    //6) ------------------------------------------------------------
    /**
     * Check critical pipelines by city. Uses a recursive algorithm that finds pipes that are likely to be critical. O(V * (V + E)).
     * @param graph
     * @return
     */
    static std::vector<WaterLossOnPipeDelete> criticalPipelines(Graph* graph);
    // ------------------------------------------------------------
    protected:
    /** @brief This helper function removes the super-source and super-sink vertices. \n
        * Output: Nothing is printed
        * Complexity: O(V)
        * @param graph The graph to remove the vertices from
    */
    static void RemoveSourceAndSink(Graph* g);
    /** @brief This helper function adds the super-source and super-sink vertices, calculating an appropriate flow for their edges from the one already present in the graph. \n
        * Output: Nothing is printed
        * Complexity: O(V) (could be made O(1) by removing the error checking)
        * @param graph The graph to add the vertices to
    */
    static void AddSourceAndSink(Graph* g);
    /** @brief This helper function resets all flow in the graph. \n
        * Output: Nothing is printed
        * Complexity: O(V + E)
        * @param graph The graph to reset the flow in
    */
    static void SetFlowToZero(Graph* graph);
    /** @brief This helper runs a slightly altered Edmonds-Karp algorithm that ignores a vertex. The graph isn't initialized or altered in any way in the beginning, so that the function can be run "in the middle" of bigger algorithms. It assumes that the super-source and super-sink exist in the graph. \n
        * Output: Nothing is printed
        * Complexity: O(V * E^2)
        * @param graph The graph to run the algorithm on
        * @param vertx The vertex to ignore
    */
    static void EdmondsKarpThatIgnoresVertex(Graph* graph,Vertex* vertx);
};


#endif //PROJECT_ALGORITHMS_H
