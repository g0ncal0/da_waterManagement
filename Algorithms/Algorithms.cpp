//
// Created by goncalo on 04-03-2024.
//

#include "Algorithms.h"
#include <algorithm>
#include <unordered_map>
#include <cmath>
using namespace std;

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

void Algorithms::simpleEdmondsKarpThatDoesntDeleteSourceAndSink(Graph *g)
{
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

    while (BFSEdmondsKarp(g, q)) {
        // Re-Initialize everything
        for (Vertex* v : g->getVertexSet()) v->setVisited(false);
        source->setVisited(true);
    }

    g->removeVertex(source);
    g->removeVertex(sink);



    // Added the calculus of the incoming water in cities.
    calculateWaterInCities(g);

    for (Vertex* v : g->getVertexSet()) {
        if (v->getType() == 'r') {
            v->setPath(nullptr);
        }
    }
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


int updatepathtoreservoir(Graph* g, Vertex* c, int excess){
    // iterate over the adj of the current vertex
    // if is a reservoir, reduce the amount of water he gives out by excess and return true;
    // if is city return false
    // divide by all the incoming
    // if return is true, continue on the remaining edges.
    // but, cycle through all adj until excess = 0
    //
    if(c->getType() == 'c'){
        // we are not updating here. water doesn't come from cities
        return 0;
    }

    if(c->getType() == 'r'){
        // its a reservatoir. Check if we can remove that amount of water.
        Reservoir* r = (Reservoir*) c;
        if(r->getDelivery() > excess){
            return excess;
        }
        return 0;
    }
    int tries = c->getAdj().size();

    int sum = 0;
    while(excess > 0 && tries > 0){
        for(Edge* edge : c->getIncoming()){
            // we will try for each edge and see how much flow we can reduce. If not possible, continue
            int howmuch = std::min(excess, (int)(edge->getFlow() * 0.4)) ; // we will try to remove 40% of the flow each time or the excess
            if(edge->getFlow() > howmuch && howmuch > 0){
                int i = updatepathtoreservoir(g, edge->getOrig(), howmuch);
                excess -= i;
                edge->setFlow(edge->getFlow() - i);
                sum +=i;
                }
            }
        }
        tries--;
    return sum;
}



void Algorithms::BalanceTheLoad(Graph* g){
    // THere is an easy way: to just do the Edmond Karp with the edge from city to sink having a capacity given by the city's need.
    // There is the hard way: to go to each city that has too much water, remove the excess and update the edges until finding a reservoir
    Menu::print("The initial statistics");
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
            int removed = 0;
            for(Edge* edge : city->getIncoming()){
                int howmuch = std::min(excess, (int)(0.4 * edge->getFlow()));
                if(howmuch > 0){
                    int r = updatepathtoreservoir(g, edge->getOrig(), excess); // understand the max amount of water we have reduced (based on capacity)
                    edge->setFlow(edge->getFlow() - r);
                    removed += r;
                    excess-= r;
                }
            }
        }
    }
    Menu::print("The end statistics");
    GlobalStatisticsEdges endstats = calculatestatistics(g);
    Menu::printStatistics(endstats.avg, endstats.max_difference, endstats.variance, endstats.n_edges);
}






std::vector<City*> Algorithms::CitiesWithNotEnoughWater(Graph* graph)
{
    std::vector<City*> cities;

    for (Vertex* vert:graph->getVertexSet())
    {
        if (vert->getType()=='c')
        {
            City* city = (City*)vert;
            if(city->getTotalWaterIn() < city->getDemand())
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


std::vector<CityWaterLoss> Algorithms::CanShutDownReservoirOptimized(Graph* graph, const std::string& reservoirCode)
{
    std::vector<CityWaterLoss> cityWaterLoss;

    std::unordered_map<City*, double> originalWaterValue; //TODO: use this to populate the above vector later.


    for (Vertex* vertex:graph->getVertexSet()) {
        if(vertex->getType()=='c'&&vertex->getCode()!="Sink")
        {
            double waterIn=0;
            for (Edge* edge:vertex->getIncoming()) {
                waterIn+=edge->getFlow();
            }


            originalWaterValue.emplace((City*)vertex,waterIn);

        }
    }

    Vertex* reservoir=graph->findVertex(reservoirCode);


//assumes it has already got all the info, including source and sink nodes, in it.

    queue<Vertex*> q;
    Vertex* source=graph->findVertex("Source");
    Vertex* sink=graph->findVertex("Sink");


double waterToRemove=0;
    for (Edge* edge:reservoir->getAdj()) {
            waterToRemove+=edge->getFlow();
    }

//TODO: pôr em diferentes funções

reservoir->setPath(nullptr);
q.push(reservoir);

    bool run = true;
//This loop is something similar to the Edmonds-Karp algorithm. It detects paths from the reservoir to the sink that have flow going through them. It removes that flow until there is no flow coming from the reservoir.

    while (run&&waterToRemove>0) {
        // Re-Initialize everything
        for (Vertex* v : graph->getVertexSet()) v->setVisited(false);
        reservoir->setVisited(true);
        q.push(reservoir);
        run=false;
        while (!q.empty()) {
            Vertex *v = q.front();
            q.pop();
            bool contnue = true;

            if (v->getCode() == "Sink") {
                int minFlow = INT_MAX; //should be double
                Vertex *vertex = v;
                Edge *edge = v->getPath();

                //this is the part that needs to change
                while (edge != nullptr) {
                    if (vertex == edge->getDest()) {
                        if ((edge->getFlow()) < minFlow)
                        { minFlow = edge->getFlow();}
                        vertex = edge->getOrig();
                    }

                    edge = vertex->getPath();
                }

                minFlow= std::min((double)minFlow,(waterToRemove));
                waterToRemove-=minFlow;

                vertex = v;
                edge = v->getPath();

                while (edge != nullptr)
                {
                    if (vertex == edge->getDest()) {
                        edge->setFlow(edge->getFlow() - minFlow);
                        vertex = edge->getOrig();
                    }

                    edge = vertex->getPath();
                }

                run = true;
                contnue = false;
                q={}; //Maybe remove
            }

            if (contnue) {
                for (Edge *edge: v->getAdj()) {
                    if ((!edge->getDest()->isVisited()) && (edge->getFlow() >0)) {
                        edge->getDest()->setPath(edge);
                        edge->getDest()->setVisited(true);
                        q.push(edge->getDest());
                    }
                }

                /*
                for (Edge *edge: v->getIncoming()) {
                    if ((!edge->getOrig()->isVisited()) && (edge->getFlow() > 0)) {
                        edge->getOrig()->setPath(edge);
                        edge->getOrig()->setVisited(true);
                        q.push(edge->getOrig());
                    }
                }
             */

            }
        }


    }




    //This is a simple edmonds-karp algorithm, except that the reservoir is being ignored.
    q.push(source);
    run = true;

    while (run) {
        // Re-Initialize everything
        for (Vertex* v : graph->getVertexSet()) {v->setVisited(false);}
        source->setVisited(true);
        run=false;
        while (!q.empty()) {
            Vertex *v = q.front();
            q.pop();
            bool contnue = true;

            if (v->getCode() == "Sink") {
                int minFlow = INT_MAX; //should be double
                Vertex *vertex = v;
                Edge *edge = v->getPath();


                while (edge != nullptr) {
                    if (vertex == edge->getDest()) {
                        if ((edge->getCapacity() - edge->getFlow()) < minFlow)
                        { minFlow = edge->getCapacity() -edge->getFlow();}
                        vertex = edge->getOrig();
                    } else {
                        if (edge->getFlow() < minFlow)
                        {minFlow = edge->getFlow();}
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
                    } else {
                        edge->setFlow(edge->getFlow() - minFlow);
                        vertex = edge->getDest();
                    }

                    edge = vertex->getPath();
                }

                run = true;
                contnue = false;
            }

            if (contnue) {

                for (Edge *edge: v->getAdj()) {
                    if(edge->getDest()==reservoir)
                    {
                        continue;
                    }
                    if ((!edge->getDest()->isVisited()) && (edge->getFlow() < edge->getCapacity())) {
                        edge->getDest()->setPath(edge);
                        edge->getDest()->setVisited(true);
                        q.push(edge->getDest());
                    }
                }

                for (Edge *edge: v->getIncoming()) {
                    if(edge->getOrig()==reservoir)
                    {
                        continue;
                    }
                    if ((!edge->getOrig()->isVisited()) && (edge->getFlow() > 0)) {
                        edge->getOrig()->setPath(edge);
                        edge->getOrig()->setVisited(true);
                        q.push(edge->getOrig());
                    }
                }
            }
        }





    }


    graph->removeVertex(source);
    graph->removeVertex(sink);

    for (std::pair<City*, double> pair:originalWaterValue)
    {
        City* first=pair.first;

        double waterIn=0;
        for (Edge* edge:first->getIncoming()) {
            waterIn+=edge->getFlow();
        }


        CityWaterLoss wl;
        wl.waterLoss= waterIn-pair.second;
        wl.cityCode=first->getCode();
        cityWaterLoss.push_back(wl);
    }


    return cityWaterLoss; //this data structure is currently not being populated

}

 void Algorithms::SetWaterIn(Graph* graph)
{
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




        }
    }
}


std::vector<WaterLossOnPipeDelete> Algorithms::criticalPipelines(Graph* graph) {
    std::vector<WaterLossOnPipeDelete> res;
    std::unordered_map<City*, double> originalWaterValue;



    return res;
}
