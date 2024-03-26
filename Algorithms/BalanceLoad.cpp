/*
//
// Created by Filipe Correia on 25/03/2024.
//
#include "Algorithms.h"
#include <queue>





bool BalanceBFSEdmondsKarp(Graph* g, std::queue<Vertex*> q) {
    while (!q.empty()) {
        Vertex* v = q.front();
        q.pop();

        if (v->getCode() == "Sink") {
            int minFlow = INT_MAX;
            Vertex* vertex = v;
            Edge* edge = v->getPath();

            while (edge != nullptr) {
                if (vertex == edge->getDest()) {
                    // IT MUST HAVE SOMETHING TO DO WITH THIS LINE:
                    if ((edge->getCapacity() * 0.7 - edge->getFlow()) < minFlow) minFlow = edge->getCapacity() * 0.7 - edge->getFlow();
                    vertex = edge->getOrig();
                }

                else {
                    if (edge->getFlow() < minFlow) minFlow = edge->getFlow() * 0.7;
                    vertex = edge->getDest();
                }

                edge = vertex->getPath();
            }

            vertex = v;
            edge = v->getPath();

            while (edge != nullptr) {
                if (vertex == edge->getDest()) {
                    edge->setFlow(edge->getFlow() + minFlow);
                    vertex = edge->getOrig();
                }
                else {
                    edge->setFlow(edge->getFlow() - minFlow);
                    vertex = edge->getDest();
                }

                edge = vertex->getPath();
            }

            return true;
        }

        double sumdif = 0;
        int n = 0;
        for(Edge* edge : v->getAdj()){
            if (!edge->getDest()->isVisited()){
                sumdif += (edge->getCapacity() - edge->getFlow()) / edge->getCapacity();
                n++;
            }
        }
        double avgdif = sumdif / n;

        for (Edge* edge : v->getAdj()) {
            if ((!edge->getDest()->isVisited()) && (edge->getFlow() < edge->getCapacity())) {
                //  the difference between flow and capacity is greater or equal than avg
                if(((edge->getCapacity() - edge->getFlow())/edge->getCapacity()) >= avgdif){
                    edge->getDest()->setPath(edge);
                    edge->getDest()->setVisited(true);
                    q.push(edge->getDest());
                }
            }
        }

        for (Edge* edge : v->getIncoming()) {
            if ((!edge->getOrig()->isVisited()) && (edge->getFlow() > 0)) {
                edge->getOrig()->setPath(edge);
                edge->getOrig()->setVisited(true);
                q.push(edge->getOrig());
            }
        }
    }

    return false;
}


// unchanged from simpleedmondskarp
void balanceEdmondsKarp(Graph *g) {
    // Creation of SuperSource and SuperSink
    std::string name = "SuperSource";
    std::string code = "Source";
    g->addReservoir(name,name,0,code,INT_MAX);

    name = "SuperSink";
    code = "Sink";
    g->addCity(name, 0, code, 0, 0);

    std::queue<Vertex*> q;
    Vertex* source;
    Vertex* sink;
    // Add edges from supersource to sources and sink to supersink + store in q the supersource for bfs
    for (Vertex* v : g->getVertexSet()) {
        v->setVisited(false);
        for (Edge* edge : v->getAdj()) edge->setFlow(0);

        if ((v->getType() == 'r') && (v->getCode() != "Source")) {
            auto* reservoir = dynamic_cast<Reservoir*>(v);
            g->addEdge("Source", v->getCode(), reservoir->getDelivery());
        }
        else if ((v->getType() == 'c') && (v->getCode() != "Sink")) {
            auto* city = dynamic_cast<City*>(v);
            g->addEdge(v->getCode(), "Sink", city->getDemand());
        }

        else if (v->getCode() == "Source") {
            v->setVisited(true);
            q.push(v);
            source = v;
        }

        else if (v->getCode() == "Sink") sink = v;
    }

    while (BalanceBFSEdmondsKarp(g, q)) {
        // Re-Initialize everything
        for (Vertex* v : g->getVertexSet()) v->setVisited(false);
        source->setVisited(true);
    }

    g->removeVertex(source);
    g->removeVertex(sink);



    // Added the calculus of the incoming water in cities.
    Algorithms::calculateWaterInCities(g);

    for (Vertex* v : g->getVertexSet()) {
        if (v->getType() == 'r') {
            v->setPath(nullptr);
        }
    }
}











*/
/***
 * Calculate statistics information of the graph - O(E + V)
 * @param g
 * @return struct with all statistics of graph
 *//*

GlobalStatisticsEdges calculatestatistics(Graph* g){
    int sum = 0;
    int howmany = 0;

    int sumforvariance = 0;

    int maxdifference = 0;
    // Calculate avg (with sum of differences) and maxdifference
    for(Vertex* vertex : g->getVertexSet()){
        for(Edge* edge : vertex->getAdj()){
            if(edge->getFlow() >= 0){
                // we guarantee that the edges we are analysing are the originals and not the "reverse"
                int difference = edge->getCapacity() - edge->getFlow();
                sum += difference;
                sumforvariance += difference * difference;
                howmany++;
                if(difference > maxdifference){
                    maxdifference = difference;
                }
            }
        }
    }
    GlobalStatisticsEdges res;

    res.avg = (float) sum / (float) howmany;
    res.variance = ((float) sumforvariance - (float) howmany * (res.avg * res.avg)) / (float) (howmany - 1); // FORMULA TO CALCULATE VARIANCE FROM M.E. (Course at LEIC).
    res.max_difference = maxdifference;
    res.n_edges = howmany;
    return res;
}




void Algorithms::BalanceTheLoad(Graph* g){
    Menu::print("The initial statistics");
    GlobalStatisticsEdges stats = calculatestatistics(g);
    Menu::printStatistics(stats.avg, stats.max_difference, stats.variance, stats.n_edges);
    */
/**
     * INITIAL IDEA: NO LONGER VIABLE
     * calculate the statistics in the beginning
     * for(all the cities)
     *  if(city capacity > demand)
     *      excess = capacity - demand;
     *      remove that excess from the capacity of the city
     *      repeat:
     *        find a path from the city to reservoir
     *        find the max weight of the path
     *        remove that weight (max(weight, excess))
     *        decrease excess by how much it was removed
     *      until excess <= 0;
     *
     * calculate statistics again
     *//*

    balanceEdmondsKarp(g);

    Menu::print("The end statistics");
    GlobalStatisticsEdges endstats = calculatestatistics(g);
    Menu::printStatistics(endstats.avg, endstats.max_difference, endstats.variance, endstats.n_edges);
}
*/
