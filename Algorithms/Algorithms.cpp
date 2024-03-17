//
// Created by goncalo on 04-03-2024.
//

#include "Algorithms.h"

using namespace std;


/**
 * A simple algorithm to calculate the water that enters every city O(n). Changes the totalwaterin var in the cities.
 * @param g graph
 */
void Algorithms::calculateWaterInCities(Graph* g){
    for(Vertex* v: g->getVertexSet()){
        if(v->getType() == 'c'){
            City* c = (City*) v;
            c->setTotalWaterIn(0);
            for(Edge* in : c->getIncoming()){
                c->setTotalWaterIn(c->getTotalWaterIn() + in->getFlow());
            }
        }
    }
}

bool Algorithms::BFSEdmondsKarp(Graph* g, queue<Vertex*> q) {
    while (!q.empty()) {
        Vertex* v = q.front();
        q.pop();

        if (v->getCode() == "Sink") {
            int minFlow = INT_MAX;
            Vertex* vertex = v;
            Edge* edge = v->getPath();

            while (edge != nullptr) {
                if (vertex == edge->getDest()) {
                    if ((edge->getCapacity() - edge->getFlow()) < minFlow) minFlow = edge->getCapacity() - edge->getFlow();
                    vertex = edge->getOrig();
                }

                else {
                    if (edge->getFlow() < minFlow) minFlow = edge->getFlow();
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

        for (Edge* edge : v->getAdj()) {
            if ((!edge->getDest()->isVisited()) && (edge->getFlow() < edge->getCapacity())) {
                edge->getDest()->setPath(edge);
                edge->getDest()->setVisited(true);
                q.push(edge->getDest());
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


void Algorithms::simpleEdmondsKarp(Graph *g) {
    // Creation of SuperSource and SuperSink
    string name = "SuperSource";
    string code = "Source";
    g->addReservoir(name,name,0,code,INT_MAX);

    name = "SuperSink";
    code = "Sink";
    g->addCity(name, 0, code, 0, 0);

    queue<Vertex*> q;
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
        else if ((v->getType() == 'c') && (v->getCode() != "Sink")) g->addEdge(v->getCode(), "Sink", INT_MAX);

        else if (v->getCode() == "Source") {
            v->setVisited(true);
            q.push(v);
            source = v;
        }

        else if (v->getCode() == "Sink") sink = v;
    }

    while (BFSEdmondsKarp(g, q)) {
        // Re-Initialize everything
        for (Vertex* v : g->getVertexSet()) v->setVisited(false);
        source->setVisited(true);
    }

    g->removeVertex(source);
    g->removeVertex(sink);


    // Added the calculus of the incoming water in cities.
    calculateWaterInCities(g);
}



//assumes that the graph has already been pre-prepared with the results of the edmonds-karp algorithm.
std::vector<CityWaterLoss> Algorithms::CanShutDownReservoir(Graph* graph, const std::string& reservoirCode)
{
    std::vector<CityWaterLoss> wl;



    //temporary algorithm, that must run edmonds-karp every time
    Graph* copy= graph->clone();

    copy->removeVertex(copy->findVertex(reservoirCode));

    simpleEdmondsKarp(copy);


    for (Vertex* vert:copy->getVertexSet())
    {
        if (vert->getType()=='c')
        {
            City* city = (City*)vert;

            double waterReceived = 0;

            for (auto incomingEdge:city->getAdj())
            {
                waterReceived+=incomingEdge->getFlow();

            }
            city->setTotalWaterIn(waterReceived);

            City *originalCity=(City*)graph->findVertex(city->getCode());

            wl.push_back({city->getCode(),city->getTotalWaterIn()-originalCity->getTotalWaterIn()});
        }

    }

    delete copy;

    return wl;
}




//assumes that the graph has already been pre-prepared with the results of the edmonds-karp algorithm.
std::vector<CityWaterLoss> Algorithms::CanShutDownReservoirs(Graph* graph, const std::vector<std::string>& reservoirCodes)
{

    std::vector<CityWaterLoss> wl;

    //temporary algorithm, that must run edmonds-karp every time
    Graph* copy= graph->clone();
    for (const auto& reservoirCode:reservoirCodes) {
        copy->removeVertex(copy->findVertex(reservoirCode));
    }
    simpleEdmondsKarp(copy);


    calculateWaterInCities(copy); // this calculates the water in

    for (Vertex* vert:copy->getVertexSet())
    {
        if (vert->getType()=='c')
        {
            City* city = (City*)vert;

            City *originalCity=(City*)graph->findVertex(city->getCode());

            wl.push_back({city->getCode(),city->getTotalWaterIn()-originalCity->getTotalWaterIn()});
        }

    }

    delete copy;

    return wl;
}



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
    // THere is an easy way: to just do the Edmond Karp with the edge from city to sink having a capacity given by the city's need.
    // There is the hard way: to go to each city that has too much water, remove the excess and update the edges until finding a reservoir
    GlobalStatisticsEdges stats = calculatestatistics(g);
    Menu::printStatistics(stats.avg, stats.max_difference, stats.variance, stats.n_edges);
    /*
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
     * */
    for(Vertex* vertex : g->getVertexSet()){
        if(vertex->getType() == 'c'){
            City* city = (City*) vertex;
            int excess = city->getTotalWaterIn() - city->getDemand();
            if(excess > 0){

            }
        }
    }


}









std::vector<City*> Algorithms::CitiesWithNotEnoughWater(Graph* graph)
{
    std::vector<City*> cities;



    for (Vertex* vert:graph->getVertexSet())
    {
        if (vert->getType()=='c')
        {
            City* city = (City*)vert;

            double waterReceived = 0;

            for (auto incomingEdge:city->getIncoming())
            {
                waterReceived+=incomingEdge->getFlow();

            }
            city->setTotalWaterIn(waterReceived);


            if(city->getTotalWaterIn()<city->getDemand())
            {
                cities.push_back(city);
            }

        }

    }
    return cities;
}





bool Algorithms::DFSShutDownReservoir(Vertex* vertex, std::vector<CityWaterLoss>& cityWaterLoss) {

    if (!vertex->getIncoming().empty()) return false; //isto é demasiado restritivo acho que dá para melhorar

    for (Edge* edge : vertex->getAdj()) {
        if (edge->getDest()->getType() == 'c') {
            // ESTAS LINHAS ERA PORQUE ESTAVA A PENSAR DE MANEIRA DIFERENTE DO QUE A GABRIELA FEZ, QUANDO ISTO FICAR FEITO EU APAGO

            /*auto* city = dynamic_cast<City*>(edge->getDest());
            int newMax = 0;

            for (Edge* cityEdge : city->getIncoming()) {
                if (cityEdge != edge) newMax += cityEdge->getCapacity();
            }

            if (newMax < city->getDemand()) {

            }*/

            for (CityWaterLoss& waterLoss : cityWaterLoss) {
                if (waterLoss.cityCode == edge->getDest()->getCode()) {
                    waterLoss.waterLoss += edge->getFlow();
                    return true;
                }
            }

            cityWaterLoss.push_back({edge->getDest()->getCode(), edge->getFlow()});
        }

        if (!DFSShutDownReservoir(edge->getDest(), cityWaterLoss)) return false;
    }

    return true;
}

std::vector<CityWaterLoss> Algorithms::smartCanShutDownReservoir(Graph* graph, const std::string& reservoirCode) {
    Vertex* vertexReservoir;

    for (Vertex* vertex : graph->getVertexSet()) {
        if (vertex->getCode() == reservoirCode) {
            vertexReservoir = vertex;
            break;
        }
    }

    std::vector<CityWaterLoss> cityWaterLoss;
    if (DFSShutDownReservoir(vertexReservoir, cityWaterLoss)) return cityWaterLoss;

    else return CanShutDownReservoir(graph, vertexReservoir->getCode());
}


#include <cmath>
using namespace std;

std::vector<WaterLossOnStationDelete> Algorithms::GetGroupsOfPumpingStationsThatCanBeRemovedSafelyBruteForce(Graph* graph)
{
    std::vector<WaterLossOnStationDelete> wl;

    std::vector<std::string> pumpingStations;
    for (Vertex* vert:graph->getVertexSet()) {
        if(vert->getType()=='s'){
        pumpingStations.push_back(vert->getCode());
        }
    }
    if(pumpingStations.size()>=64)
    {
        std::cout<< "Error: too many stations for the brute force implementation";
        return wl;
    }

    for (unsigned long i = 0; i < pow(2,pumpingStations.size()); ++i) {
        std::vector<std::string> pumpingStationsToRemove;
        for (unsigned long j = 0; j < pumpingStations.size(); ++j) {
            unsigned long mask= (1<<j);
            if(i&mask)
            {
                pumpingStationsToRemove.push_back(pumpingStations[j]);
            }

        }

        std::vector<CityWaterLoss> wl2;

        Graph* copy= graph->clone();
        for (const auto& code:pumpingStationsToRemove) {
            copy->removeVertex(copy->findVertex(code));
        }
        simpleEdmondsKarp(copy);

        for (Vertex* vert:copy->getVertexSet())
        {
            if (vert->getType()=='c')
            {
                City* city = (City*)vert;

                double waterReceived = 0;

                for (auto incomingEdge:city->getAdj())
                {
                    waterReceived+=incomingEdge->getFlow();

                }
                city->setTotalWaterIn(waterReceived);

                City *originalCity=(City*)graph->findVertex(city->getCode());

                wl2.push_back({city->getCode(),city->getTotalWaterIn()-originalCity->getTotalWaterIn()});
            }

        }

        delete copy;
        WaterLossOnStationDelete loss;
        loss.deletedStationsCodes=pumpingStationsToRemove;
        loss.waterLoss=wl2;
        wl.push_back(loss);


    }



return wl;
}



std::vector<WaterLossOnPipeDelete> Algorithms::GetGroupsOfEdgesThatCanBeRemovedSafelyBruteForce(Graph* graph)
{
    std::vector<WaterLossOnPipeDelete> wl;


    std::vector<PipeStartEndCodes> allPipes;

    for (Vertex* vert:graph->getVertexSet()) {
        for (auto edge:vert->getAdj())
        {
            allPipes.push_back({edge->getOrig()->getCode(),edge->getDest()->getCode()});
        }
    }

    if(allPipes.size()>=64)
    {
        std::cout<< "Error: too many stations for the brute force implementation";
        return wl;
    }

    for (unsigned long i = 0; i < pow(2,allPipes.size()); ++i) {

        std::vector<PipeStartEndCodes> deletedPipesCodes;
        for (unsigned long j = 0; j < allPipes.size(); ++j) {
            unsigned long mask= (1<<j);
            if(i&mask)
            {
                deletedPipesCodes.push_back(allPipes[j]);
            }

        }


        std::vector<CityWaterLoss> wl2;

        Graph* copy= graph->clone();
        for (const auto& codes:deletedPipesCodes) {
            copy->removeEdge(codes.first,codes.second);
        }

        simpleEdmondsKarp(copy);

        for (Vertex* vert:copy->getVertexSet())
        {
            if (vert->getType()=='c')
            {
                City* city = (City*)vert;

                double waterReceived = 0;

                for (auto incomingEdge:city->getAdj())
                {
                    waterReceived+=incomingEdge->getFlow();

                }
                city->setTotalWaterIn(waterReceived);

                City *originalCity=(City*)graph->findVertex(city->getCode());

                wl2.push_back({city->getCode(),city->getTotalWaterIn()-originalCity->getTotalWaterIn()});
            }

        }

        delete copy;
        WaterLossOnPipeDelete loss;
        loss.deletedPipesCodes=deletedPipesCodes;
        loss.waterLoss=wl2;
        wl.push_back(loss);



    }

    return wl;

}