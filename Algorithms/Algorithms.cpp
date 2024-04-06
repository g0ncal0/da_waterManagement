//
// Created by goncalo on 04-03-2024.
//

#include "Algorithms.h"
#include <algorithm>
#include <unordered_map>
#include <cmath>
#include <sstream>

using namespace std;

using namespace std;



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

    queue<Vertex*> q;
    Vertex* source;
    // Add edges from supersource to sources and sink to supersink + store in q the supersource for bfs
    for (Vertex* v : g->getVertexSet()) {
        v->setVisited(false);
        v->setPath(0); //todo: remove this?
        for (Edge* edge : v->getAdj()) edge->setFlow(0); //todo: remove this?

        if (v->getCode() == "Source") {
            v->setVisited(true);
            q.push(v);
            source = v;
        }

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
                    if (edge->getReverse() == nullptr) edge->setFlow(edge->getFlow() + minFlow);

                    else { //tratamento de edges bidirecionais
                        int flow = edge->getFlow();
                        int reverse_flow = edge->getReverse()->getFlow();

                        if (reverse_flow == 0) edge->setFlow(flow + minFlow);
                        else if (reverse_flow >= minFlow) edge->getReverse()->setFlow(reverse_flow - minFlow);
                        else {
                            edge->setFlow(minFlow - reverse_flow);
                            edge->getReverse()->setFlow(0);
                        }
                    }

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

    std::unordered_map<City*, int> initialValues;

    for (Vertex* vertex:graph->getVertexSet()) {
        if (vertex->getType()=='c'&&vertex->getCode()!="Sink")
        {
            initialValues.emplace((City*)vertex,((City*)vertex)->getTotalWaterIn());
        }
        for (Edge* edge:vertex->getAdj()) {
            edge->setFlow(0);
        }
    }
 //this isn't setting the flow?
    EdmondsKarpThatIgnoresVertex(graph,graph->findVertex(reservoirCode));

    for(auto& pr:initialValues)
    {
        int win=0;
        for (Edge* edge:pr.first->getIncoming()) {
            win+=edge->getFlow();
        }

        CityWaterLoss cwl;
        cwl.cityCode=pr.first->getCode();
        cwl.waterLoss= win-pr.second;
        wl.push_back(cwl);
    }


    return wl;
}


/**
 * Calculate statistics information of the graph - O(E + V)
 * @param g
 * @return struct with all statistics of graph
 */
GlobalStatisticsEdges calculatestatistics(Graph* g){
    for (Vertex* vertex : g->getVertexSet()) {
        for (Edge* edge : vertex->getAdj()) {
            edge->setSelected(false);
        }
    }

    int sum = 0;
    int howmany = 0;
    int sumforvariance = 0;
    int maxdifference = 0;
    // Calculate avg (with sum of differences) and maxdifference
    for(Vertex* vertex : g->getVertexSet()){
        for(Edge* edge : vertex->getAdj()){
            if ((edge->getFlow() == 0) && (edge->getReverse() != nullptr) && (!edge->getReverse()->isSelected())) {
                edge->setSelected(true);
                continue;
            }

            else if (!edge->isSelected()) {
                edge->setSelected(true);
                if (edge->getReverse() != nullptr) edge->getReverse()->setSelected(true);
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

void Algorithms::ChooseBalanceTheLoad(Graph* g) {
    int choice;
    cout << "Which algorithm you want to use?\n1. It is simpler but may have worst results in Large Data Set\n2. It is heavier but gets better results in Large Data Set\n";
    cin >> choice;
    if (choice == 1) BalanceTheLoad(g);
    else BalanceTheLoad2(g);
}

void Algorithms::BalanceTheLoad(Graph* g){
    Menu::print("The initial statistics");
    GlobalStatisticsEdges stats = calculatestatistics(g);
    Menu::printStatistics(stats.avg, stats.max_difference, stats.variance, stats.n_edges);

    for (Vertex* vertex : g->getVertexSet()) {
        vertex->setVisited(false);
        vertex->setPath(nullptr);
    }

    for (Vertex* vertex : g->getVertexSet()) {
        if (vertex->getIncoming().size() <= 1) continue;
        int space = 0;

        for (Edge* edge : vertex->getIncoming()) {
            space += edge->getCapacity() - edge->getFlow();
        }

        int mean_space = space / (int)vertex->getIncoming().size();

        for (Edge* edge : vertex->getIncoming()) {
            int edge_space = edge->getCapacity() - edge->getFlow();

            if (edge_space < mean_space) {
                int maxFlow = mean_space - edge_space;
                int improved = 0;
                queue<Vertex*> q;
                Vertex* source = edge->getOrig();
                q.push(source);
                source->setVisited(true);

                while (auxBFSBalanceTheLoad(g, q, source->getCode(), vertex->getCode(), maxFlow)) {
                    // Re-Initialize everything
                    for (Vertex* v : g->getVertexSet()) v->setVisited(false);

                    improved += maxFlow;
                    edge->setFlow(edge->getFlow() - maxFlow);
                    if ((improved == space) || (maxFlow == 0)) break;
                    maxFlow = space - improved;

                    source->setVisited(true);
                    source->setPath(nullptr);
                }
            }
        }
    }


    Menu::print("The end statistics");
    GlobalStatisticsEdges endstats = calculatestatistics(g);
    Menu::printStatistics(endstats.avg, endstats.max_difference, endstats.variance, endstats.n_edges);
}

bool Algorithms::auxBFSBalanceTheLoad(Graph* g, std::queue<Vertex*> q, const std::string& source, const std::string& sink, int& maxFlow) {
    while (!q.empty()) {
        Vertex* v = q.front();
        q.pop();

        if (v->getCode() == sink) {
            int minFlow = maxFlow;
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

                if (vertex->getCode() == source) break;

                edge = vertex->getPath();
            }

            vertex = v;
            edge = v->getPath();
            maxFlow = minFlow;

            while (edge != nullptr) {
                if (vertex == edge->getDest()) {
                    if ((edge->getFlow() == 0) && (edge->getReverse() != nullptr) && (edge->getReverse()->getFlow() > 0)) {
                        edge->getReverse()->setFlow(edge->getReverse()->getFlow() - minFlow);
                    }

                    else {
                        edge->setFlow(edge->getFlow() + minFlow);
                    }

                    vertex = edge->getOrig();
                }
                else {
                    if ((edge->getFlow() == 0) && (edge->getReverse() != nullptr) && (edge->getReverse()->getFlow() > 0)) {
                        edge->getReverse()->setFlow(edge->getReverse()->getFlow() + minFlow);
                    }

                    else {
                        edge->setFlow(edge->getFlow() - minFlow);
                    }

                    vertex = edge->getDest();
                }

                if (vertex->getCode() == source) break;

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

void Algorithms::BalanceTheLoad2(Graph* g) {
    Menu::print("The initial statistics");
    GlobalStatisticsEdges stats = calculatestatistics(g);
    Menu::printStatistics(stats.avg, stats.max_difference, stats.variance, stats.n_edges);

    for (Vertex* vertex : g->getVertexSet()) {
        vertex->setVisited(false);
        vertex->setPath(nullptr);
    }

    for (Vertex* vertex : g->getVertexSet()) {
        if (vertex->getIncoming().size() <= 1) continue;

        for (Edge* edge : vertex->getIncoming()) {
            int edge_space = edge->getCapacity() - edge->getFlow();

            if (edge_space < (int)stats.avg) {
                //tirar água que está a stressar a rede
                int goal = (int)stats.avg - edge_space;
                vector<Edge*> path;
                path.push_back(edge);
                goal = min(goal, (int)edge->getFlow());
                goal = min(goal, findPathToReservoir(g, edge->getOrig(), path));
                if (goal == 0) continue;
                for (Edge* edge1 : path) edge1->setFlow(edge1->getFlow() - goal);

                //tentar encontrar outro caminho para a água
                for (Edge* edge2 : vertex->getIncoming()) {
                    int edge2_space = edge2->getCapacity() - edge2->getFlow();

                    if (edge2_space > (int)stats.avg) {
                        int goal2 = edge2_space - (int)stats.avg;
                        goal2 = min(goal, goal2);
                        goal2 = min(goal2, (int)edge2->getCapacity() - (int)edge2->getFlow());
                        vector<Edge*> path2;
                        path2.push_back(edge2);
                        goal2 = min(goal2, findAugmentationPathToReservoir(g, edge2->getOrig(), path2));
                        if (goal2 == 0) continue;
                        for (Edge* edge1 : path2) edge1->setFlow(edge1->getFlow() + goal2);
                        goal -= goal2;
                        if (goal == 0) break;
                    }
                }

                if (goal > 0) {
                    for (Edge* edge1 : path) edge1->setFlow(edge1->getFlow() + goal);
                }
            }
        }
    }

    Menu::print("The end statistics");
    GlobalStatisticsEdges endstats = calculatestatistics(g);
    Menu::printStatistics(endstats.avg, endstats.max_difference, endstats.variance, endstats.n_edges);
}

int Algorithms::findPathToReservoir(Graph* g, Vertex* origin, vector<Edge*>& path) {
    origin->setVisited(true);
    queue<Vertex*> q;
    q.push(origin);
    Vertex* reservoir = nullptr;

    while (!q.empty() && (reservoir == nullptr)) {
        Vertex* vertex = q.front();
        q.pop();

        for (Edge* edge : vertex->getIncoming()) {
            if ((!edge->getOrig()->isVisited()) && (edge->getFlow() > 49)) {
                if (edge->getOrig()->getType() == 'r') {
                    edge->getOrig()->setPath(edge);
                    reservoir = vertex;
                    break;
                }

                edge->getOrig()->setVisited(true);
                edge->getOrig()->setPath(edge);
                q.push(edge->getOrig());
            }
        }
    }

    for (Vertex* vertex : g->getVertexSet()) vertex->setVisited(false);
    if (reservoir == nullptr) return 0;

    Vertex* aux = reservoir;
    int minFlow = INT_MAX;

    while (aux != origin) {
        if (aux->getPath()->getFlow() < minFlow) minFlow = aux->getPath()->getFlow();
        path.push_back(aux->getPath());
        aux = aux->getPath()->getDest();
    }

    return minFlow;
}

int Algorithms::findAugmentationPathToReservoir(Graph* g, Vertex* origin, std::vector<Edge*>& path) {
    origin->setVisited(true);
    queue<Vertex*> q;
    q.push(origin);
    Vertex* reservoir = nullptr;

    while (!q.empty() && (reservoir == nullptr)) {
        Vertex* vertex = q.front();
        q.pop();

        for (Edge* edge : vertex->getIncoming()) {
            if ((!edge->getOrig()->isVisited()) && ((edge->getCapacity() - edge->getFlow()) > 49) && ((edge->getReverse() == nullptr) || (edge->getFlow() > 0))) {
                if (edge->getOrig()->getType() == 'r') {
                    edge->getOrig()->setPath(edge);
                    reservoir = vertex;
                    break;
                }

                edge->getOrig()->setVisited(true);
                edge->getOrig()->setPath(edge);
                q.push(edge->getOrig());
            }
        }
    }

    for (Vertex* vertex : g->getVertexSet()) vertex->setVisited(false);
    if (reservoir == nullptr) return 0;

    Vertex* aux = reservoir;
    int minFlow = INT_MAX;

    while (aux != origin) {
        int capacity = aux->getPath()->getCapacity() - aux->getPath()->getFlow();

        if (capacity < minFlow) minFlow = capacity;
        path.push_back(aux->getPath());
        aux = aux->getPath()->getDest();
    }

    return minFlow;
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






void RemoveWaterFromVertexToSink(Graph* graph,Vertex* vertex)
{   queue<Vertex*> q;

    double waterToRemove=0;
    for (Edge* edge:vertex->getAdj()) {
        waterToRemove+=edge->getFlow();
    }

    vertex->setPath(nullptr);
    q.push(vertex);

    bool run = true;
//This loop is something similar to the Edmonds-Karp algorithm. It detects paths from the vertex to the sink that have flow going through them. It removes that flow until there is no flow coming from the vertex.

    while (run&&waterToRemove>0) {
        // Re-Initialize everything
        for (Vertex* v : graph->getVertexSet()) {v->setVisited(false);}
        vertex->setVisited(true);
        q.push(vertex);
        run=false;
        while (!q.empty()) {
            Vertex *v = q.front();
            q.pop();
            bool contnue = true;

            if (v->getCode() == "Sink") {
                int minFlow = INT_MAX; //should be double
                Vertex *vert = v;
                Edge *edge = v->getPath();

                //this is the part that needs to change
                while (edge != nullptr) {
                    if (vert == edge->getDest()) {
                        if ((edge->getFlow()) < minFlow)
                        { minFlow = edge->getFlow();}
                        vert = edge->getOrig();
                    }

                    edge = vert->getPath();
                }

                minFlow= std::min((double)minFlow,(waterToRemove));
                waterToRemove-=minFlow;

                vert = v;
                edge = v->getPath();

                while (edge != nullptr)
                {
                    if (vert == edge->getDest()) {
                        edge->setFlow(edge->getFlow() - minFlow);
                        vert = edge->getOrig();
                    }

                    edge = vert->getPath();
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

}


void Algorithms::EdmondsKarpThatIgnoresVertex(Graph* graph,Vertex* vertx)//and doesn't do initialization
{
    queue<Vertex*> q;
    bool run=true;
    Vertex* source=graph->findVertex("Source");

    run = true;

    while (run) {
        // Re-Initialize everything
        for (Vertex* v : graph->getVertexSet()) {v->setVisited(false);}
        source->setVisited(true);
        q={};
        q.push(source);
        run=false;
        while (!q.empty()&&!run) {
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
                        if(edge->getReverse()==nullptr){
                        edge->setFlow(edge->getFlow() + minFlow);
                        } else
                        {
                            int flow =edge->getFlow();
                            int reverseFlow=edge->getReverse()->getFlow();

                            if (reverseFlow==0)
                            {
                                edge->setFlow(flow+minFlow);
                            }else if (reverseFlow >=minFlow)
                            {
                                edge->getReverse()->setFlow(reverseFlow-minFlow);
                            }else{
                                edge->setFlow(minFlow-reverseFlow);
                                edge->getReverse()->setFlow(0);
                            }
                        }
                        vertex = edge->getOrig();

                    } else {
                       // if (edge->getReverse()==0) {
                            edge->setFlow(edge->getFlow() - minFlow);
                       /* }else
                        {
                            int flow =edge->getFlow();
                            int reverseFlow=edge->getReverse()->getFlow();
                            //this part is completely wrong anyway
                            if (flow==0)
                            {
                                edge->getReverse()->setFlow(flow+minFlow);
                            }else if (flow >=minFlow)
                            {
                                edge->getReverse()->setFlow(reverseFlow-minFlow);
                            }else{
                                edge->setFlow(minFlow-reverseFlow);
                                edge->getReverse()->setFlow(0);
                            }

                        }
*/
                        vertex = edge->getDest();
                    }

                    edge = vertex->getPath();
                }

                run = true;
                contnue = false;
            }

            if (contnue) {

                for (Edge *edge: v->getAdj()) {
                    if(edge->getDest() == vertx)
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
                    if(edge->getOrig() == vertx)
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


}



void Algorithms::shutDownReservoir(Graph* graph){

    // Get reservoir
    std::string reservTOREMOVE = Menu::getInput("Code of reservoir to remove");

    // Do the computation
    Algorithms::AddSourceAndSink(graph);
    Algorithms::simpleEdmondsKarpThatDoesntDeleteSourceAndSink(graph);
    Algorithms::calculateWaterInCities(graph); // must be called for the non-optimized algorithm


    auto res1= Algorithms::CanShutDownReservoirOptimized(graph,reservTOREMOVE);



    // To display to user
    std::stringstream re;

    for (auto& r1:res1)
    {
        if (r1.waterLoss!=0) {
            re << "Difference found at " << r1.cityCode << " : " << r1.waterLoss << "\n";
        }
    }

    if (re.str()=="")
    {
        re<<"The water delivery wasn't affected!";
    }
    Menu::print(re.str());
    Algorithms::RemoveSourceAndSink(graph);
}

#include <unordered_map>
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

    if (!reservoir)
    {
        cout<<"Invalid reservoir";
        return {};
    }
    if (reservoir->getType()!='r')
    {
        cout<<"Invalid reservoir";
        return {};
    }

//assumes it has already got all the info, including source and sink nodes, in it.

    Vertex* source=graph->findVertex("Source");
    Vertex* sink=graph->findVertex("Sink");


    RemoveWaterFromVertexToSink(graph,reservoir);


    //This is a simple edmonds-karp algorithm, except that the reservoir is being ignored.


    EdmondsKarpThatIgnoresVertex(graph,reservoir);



  //  graph->removeVertex(source);
  //  graph->removeVertex(sink);

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


    return cityWaterLoss;

}



void RemoveWaterFromSourcesToVertex(Graph* graph,Vertex* vertex)
{
    queue<Vertex*> q;

    double waterToRemove=0;
    for (Edge* edge:vertex->getIncoming()) {
        waterToRemove+=edge->getFlow();
    }

    vertex->setPath(nullptr);
    q.push(vertex);

    bool run = true;

    while (run&&waterToRemove>0) {
        // Re-Initialize everything
        for (Vertex* v : graph->getVertexSet()) v->setVisited(false);
        vertex->setVisited(true);
        q.push(vertex);
        run=false;
        while (!q.empty()) {
            Vertex *v = q.front();
            q.pop();
            bool contnue = true;

            if (v->getCode() == "Source") {
                int minFlow = INT_MAX; //should be double
                Vertex *vert = vertex;
                Edge *edge = v->getPath();

                //this is the part that needs to change
                while (edge != nullptr) {
                    if (vertex == edge->getDest()) {
                        if ((edge->getFlow()) < minFlow)
                        { minFlow = edge->getFlow();}
                        vert = edge->getOrig();
                    }

                    edge = vert->getPath();
                }

                minFlow= std::min((double)minFlow,(waterToRemove));
                waterToRemove-=minFlow;

                vert = vertex;
                edge = v->getPath();

                while (edge != nullptr)
                {
                    if (vert == edge->getDest()) {
                        edge->setFlow(edge->getFlow() - minFlow);
                        vert = edge->getOrig();
                    }

                    edge = vert->getPath();
                }

                run = true;
                contnue = false;
                q={}; //Maybe remove
            }

            if (contnue) {
                /*
                for (Edge *edge: v->getAdj()) {
                    if ((!edge->getDest()->isVisited()) && (edge->getFlow() >0)) {
                        edge->getDest()->setPath(edge);
                        edge->getDest()->setVisited(true);
                        q.push(edge->getDest());
                    }
                }*/

                for (Edge *edge: v->getIncoming()) {
                    if ((!edge->getOrig()->isVisited()) && (edge->getFlow() > 0)) {
                        edge->getOrig()->setPath(edge);
                        edge->getOrig()->setVisited(true);
                        q.push(edge->getOrig());
                    }
                }

            }
        }


    }

}



//This one probably won't be used in the end...
std::vector<CityWaterLoss> Algorithms::CanDeletePumpingStationFrom0(Graph* graph, const std::string& stationCode)
{
    std::vector<CityWaterLoss> cityWaterLoss;
    std::unordered_map<City*, double> originalWaterValue; //TODO: use this to populate the above vector later.

    for (Vertex* vertex : graph->getVertexSet() ) {

        if (vertex->getType()=='c'&&vertex->getCode()!="Sink")
        {
            City* city=(City*) vertex;
            originalWaterValue.emplace(city,city->getTotalWaterIn());
        }
        for (Edge* e:vertex->getAdj()) {
            e->setFlow(0);
        }
    }

    EdmondsKarpThatIgnoresVertex(graph,graph->findVertex(stationCode));


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


    return cityWaterLoss;
}


void Algorithms::deletePumpingStation(Graph *graph) {

    std::string pumpStation = Menu::getInput("Code of pumping station to remove");
    Algorithms::AddSourceAndSink(graph);
    Algorithms::SetFlowToZero(graph);
    Algorithms::simpleEdmondsKarpThatDoesntDeleteSourceAndSink(graph);
    Algorithms::calculateWaterInCities(graph);
    /*
    for (Vertex* vert:graph->getVertexSet()) {
        for (Edge* edge: vert->getAdj()) {
            if (edge->getReverse())
            {
                if(edge->getFlow()&&edge->getReverse()->getFlow())
                {
                    cout<<"Error!!\n";
                }
            }
        }
    }
*/
    auto res3=Algorithms::CanDeletePumpingStationOptimized(graph,pumpStation);
/*
    for (Vertex* vert:graph->getVertexSet()) {
        for (Edge* edge: vert->getAdj()) {
            if (edge->getReverse())
            {
                if(edge->getFlow()&&edge->getReverse()->getFlow())
                {
                    cout<<"Error!!\n";
                }
            }
        }
    }
*/

    std::stringstream stream;
    for (auto& r1:res3) {
            if (r1.waterLoss != 0) {
                stream << "Difference found: " << r1.cityCode << ", "<<r1.waterLoss<< "\n";
            }
    }
    if (stream.str()=="")
    {
        stream<<"The water delivery wasn't affected!";
    }
    Menu::print(stream.str());

    Algorithms::RemoveSourceAndSink(graph);
}



std::vector<CityWaterLoss> Algorithms::CanDeletePumpingStationOptimized(Graph* graph, const std::string& stationCode)
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


    Vertex* station= graph->findVertex(stationCode);
    if (!station)
    {
        cout<<"Pumping station not found!\n";
        return {};
    }
    if(station->getType()!='s')
    {
        cout<<"Vertex wasn't a pumping station";
        return {};
    }


    RemoveWaterFromVertexToSink(graph, station);
    RemoveWaterFromSourcesToVertex(graph,station);
    EdmondsKarpThatIgnoresVertex(graph,station);
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


    return cityWaterLoss;
}


//Doesn't change flow of existing edges...
void Algorithms::AddSourceAndSink(Graph* graph)
{

    // Creation of SuperSource and SuperSink
    string name = "SuperSource";
    string code = "Source";
    graph->addReservoir(name,name,0,code,INT_MAX);

    name = "SuperSink";
    code = "Sink";
    graph->addCity(name, 0, code, 0, 0);
    for (Vertex* v : graph->getVertexSet()) {

        if ((v->getType() == 'r') && (v->getCode() != "Source")) {
            auto* reservoir = dynamic_cast<Reservoir*>(v);
            int outgoingFlow=0;
            for (Edge* edge: v->getAdj()) {
                outgoingFlow+=edge->getFlow();
            }

            graph->addEdge("Source", v->getCode(), reservoir->getDelivery());
            for (Edge* edge: v->getIncoming()) {
                if(edge->getOrig()->getCode()=="Source")
                {
                    edge->setFlow(outgoingFlow);
                    break;
                }
            }
        }
        else if ((v->getType() == 'c') && (v->getCode() != "Sink"))
        {
                int incomingFlow=0;
            for (Edge* edge: v->getIncoming()) {
                incomingFlow+=edge->getFlow();
            }

            graph->addEdge(v->getCode(), "Sink", ((City*)v)->getDemand());
            for (Edge* edge: v->getAdj()) {
                if(edge->getDest()->getCode()=="Sink")
                {
                    edge->setFlow(incomingFlow);
                    break;
                }
            }
        }
    }

}




/**
 * Computes if any of the edges that is entering the vertex given is critical based on local information
 * REQUIRES RUNNING EDMONDS KARP BEFORE
 * @param vertex
 * @param required
 * @param vector the vector which is updated everytime it's found a new critical edge
 * @return
 */
void checkcritical(Vertex* vertex, int required, vector<Edge*>* vector){
    if(vertex->getType() == 'r'){
        // We are in a reservoir, end!
        return;
    }
    int sum = 0;

    for(Edge* e : vertex->getIncoming()) {
        sum += e->getCapacity();
    }

    for(Edge* current : vertex->getIncoming()){

        if(current->getFlow() == 0){
            // definitely, not critical -> we didn't need it for edmonds karp
            continue;
        }

        int sumothers = sum - current->getCapacity();
        if(sumothers < required){
            // if others can't give everything that is required, then this is critical!!
            vector->push_back(current);
            checkcritical(current->getOrig(), required-sumothers,vector);
        }
    }

}





void Algorithms::criticalPipelines(Graph* graph) {
    std::unordered_map<City*, double> originalWaterValue;


    Menu::print("FOR THIS ALGORITHM, YOU NEED TO HAVE HAD RUN EDMONDS KARP (option 2)!");
    /*
     * for all cities:
     *  check if demands are met
     *  if so, checkcritical(city, demand, vector),
     */


    // HERE: I must update a attribute max-flow at edge so that i can understand how much a edge can really bring to table if needed
    // and understand if others are critical


    for(Vertex* v : graph->getVertexSet()){
        if(v->getType() == 'c'){
            City* city = (City*) v;
            if(city->getDemand() == city->getTotalWaterIn()){
                vector<Edge*> result;
                checkcritical(v, city->getDemand(), &result);

                Menu::print("--- CITY NAME: " + city->getName() + " (" + city->getCode() + ")");
                std::stringstream edges;
                for(Edge* e : result){
                    edges <<  e->getOrig()->getCode() << " to " << e->getDest()->getCode() << " | ";
                }
                Menu::print(edges.str());
            }
        }
    }
}

/**
 * Update vertexes that are after current vertex, by the min of howmuch and the flow of the edge to that vertex
 * @param howmuch
 * @param current the vertex to start analysing
 */
void updateFollowingPaths(int howmuch, Vertex* current){


    // Check if current is valid
    if(current->getType() == 'r'){
        return;
    }
    current->setVisited(true);

    // Set current to howmuch
    if(current->getNeedLack() + howmuch < 0){
        current->setNeedLack(0);
    }else{
        current->setNeedLack(current->getNeedLack() + howmuch);
    }


    // Iterate over all adjacent - and updateFollowing(min(edge->flow, howmuch), edge->dest)
    for(Edge* adj : current->getAdj()){
        if(!adj->getDest()->isVisited()){
            if(std::min((int) adj->getFlow(), howmuch) != 0){
                updateFollowingPaths(std::min( (int) adj->getFlow(), howmuch), adj->getDest());
            }
        }
    }

}

bool findPath(Vertex* searching, int* howmuch, int allowed){

    searching->setVisited(true);

    if(searching->getNeedLack() != 0){
        // We will remove from search the amount of water we were already able to take somewhere (where needed)
        int update = std::min(*howmuch, std::min((int)(searching->getNeedLack()), allowed));
        *howmuch -= update;

        // if I found something, update following paths with negative.
        updateFollowingPaths(-update, searching); // this will update all vertexes reached by that vertex
    }

    for(Edge* e : searching->getAdj()){
        if(e->isSelected()){
            if(*howmuch > 0 && !(e->getDest()->isVisited())){
                findPath(e->getDest(), howmuch, std::min(allowed, (int)( e->getCapacity() - e->getFlow())));
            }
        }
    }

    if(*howmuch == 0){
        return true;
    }

    return false;
}


void Algorithms::PipelineDeletionImpact(Graph* graph){
    // FINDING PIPELINE THAT IT REFERS TO
    Menu::print("To guarantee good results, run Edmonds Karp before this algorithm!");

    std::string pipeorigin = Menu::getInput("Code of origin of pipe:");
    Menu::print("Code of destination of pipe");
    // A minor bug forces us to use input this way.
    std::string pipeend;
    std::cin >> pipeend;

    Vertex* origin = graph->findVertex(pipeorigin);
    Vertex* dest = nullptr;
    Edge* pipeline = nullptr;
    if(origin == nullptr){
        Menu::print("ERROR: Origin not found");
        return;
    }
    for(Edge* e : origin->getAdj()){
        if(e->getDest()->getCode() == pipeend){
            dest = e->getDest();
            pipeline = e;
        }
    }
    if(dest == nullptr){
        Menu::print("ERROR: NOT FOUND");
        return;
    }
    pipeline->setSelected(true);

    for(Vertex* v :graph->getVertexSet()){
        v->setVisited(false);
    }
    updateFollowingPaths(pipeline->getFlow(), dest);


    for(Vertex* v :graph->getVertexSet()){
        v->setVisited(false);
    }

    // find paths from past
    int qty = pipeline->getFlow();

    if(findPath(origin, &qty, pipeline->getFlow())){
        Menu::print("We were able to find a way of redestributing the water using the simple algorithm.");
        Menu::print("We have reasons to believe this edge is not critical.");
    }else {
        Menu::print("We were NOT able to find a way of redestributing the water using the simple algorithm.");
        std::cout << "It remains " << qty << " lt of water to reach cities. ";

        Menu::print("These are the cities we have reasons to believe will be affected:");
        for(Vertex* v: graph->getVertexSet()){
            if(v->getType() == 'c'){
                City* c = (City*) v;
                if(v->getNeedLack() > 0){
                    std::cout << v->getCode() + " (" + c->getName() + ") | MAX AMOUNT OF LOSS: " << v->getNeedLack() << "l \n";
                }
            }
        }

        Menu::print("We will run Edmonds Karp for test.");
        pipeline->setCapacity(0);
        Algorithms::simpleEdmondsKarp(graph);

        Menu::print("This are the resulting cities with not enough water");
        Menu::printCities(Algorithms::CitiesWithNotEnoughWater(graph));
    }


}


void Algorithms::SetFlowToZero(Graph* graph)
{
    for (Vertex* vert:graph->getVertexSet()) {
        for (Edge* edge:vert->getAdj()) {
            edge->setFlow(0);
        }

    }

}
void Algorithms::RemoveSourceAndSink(Graph* g)
{
    g->removeVertex(g->findVertex("Sink"));
    g->removeVertex(g->findVertex("Source"));
}