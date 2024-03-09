#include <iostream>
#include "Menu.h"
#include "graph/Graph.h"
#include "Parser.h"
#include "Algorithms/Algorithms.h"
using namespace std;

int main() {
    //  Menu::print("hello");
    //

    Parser p;
    Graph* graph=p.parse("../data/Small/Reservoirs_Madeira.csv",
                         "../data/Small/Stations_Madeira.csv",
                         "../data/Small/Cities_Madeira.csv",
                         "../data/Small/Pipes_Madeira.csv");

    Algorithms::simpleEdmondsKarp(graph);
    std::vector<City*> cities = Algorithms::CitiesWithNotEnoughWater(graph);
    //Algorithms::CanShutDownReservoir(graph, "R_4");
    /*
    Menu::displayoptions();
    int c = true;
    while(c){
        int i = Menu::chooseoption();
        switch(i){
            case 0:
                Algorithms::simpleEdmondsKarp(graph);
                break;
            case 1:
                c = false;
                break;
        }
    }

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
    }*/
    return 0;
}
