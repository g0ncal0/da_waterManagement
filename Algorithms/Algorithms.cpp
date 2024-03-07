//
// Created by goncalo on 04-03-2024.
//

#include "Algorithms.h"

using namespace std;

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
    string name = "SuperSource";
    string code = "Source";
    g->addReservoir(name,name,0,code,INT_MAX);

    name = "SuperSink";
    code = "Sink";
    g->addCity(name, 0, code, 0, 0);

    queue<Vertex*> q;
    Vertex* source;
    Vertex* sink;

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
        for (Vertex* v : g->getVertexSet()) v->setVisited(false);
        source->setVisited(true);
    }

    g->removeVertex(source);
    g->removeVertex(sink);
}



vector<City*> Algorithms::CitiesWithNotEnoughWater(Graph* g)
{
    //simpleEdmondsKarp(g);
    vector<City*>info;
    
    for (Vertex* vert:g->getVertexSet())
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
            info.push_back(city);
        }
        
    }
    return info;
}

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
