#include <iostream>
#include "Menu.h"
#include "graph/Graph.h"
#include "Parser.h"
#include "Algorithms/Algorithms.h"
using namespace std;

int main() {
    //  Menu::print("hello");
    //  Menu::displayoptions();


    Parser p;
    Graph* graph=p.parse("../data/Small/Reservoirs_Madeira.csv",
                         "../data/Small/Stations_Madeira.csv",
                         "../data/Small/Cities_Madeira.csv",
                         "../data/Small/Pipes_Madeira.csv");

    //std::cout << "Hello, World!" << std::endl;

    Algorithms::simpleEdmondsKarp(graph);

    cout << graph->getVertexSet().size() << endl;

    for (Vertex* v : graph->getVertexSet()) {
        cout << v->getCode();
        if (v->getType() == 'r') {
            Reservoir* r = dynamic_cast<Reservoir*>(v);
            cout << "   " << r->getDelivery();
        }
        cout << endl;

        for (Edge* edge : v->getAdj()) {
            cout << "   " << edge->getDest()->getCode() << "    " << edge->getCapacity() << "   " << edge->getFlow() << endl;
        }
    }

    Graph g;
    string city = "Porto";
    string code = "C1";
    g.addCity(city, 1, code, 10, 1000);
    g.findCity("Porto");
    return 0;
}
