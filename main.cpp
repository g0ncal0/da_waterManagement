#include <iostream>
#include "Menu.h"
#include "graph/Graph.h"
#include "Parser.h"
#include "Algorithms/Algorithms.h"
using namespace std;

int main() {
    //  Menu::print("hello");
    //
    Graph *graph;
try {
    Parser p;
    graph = p.parse("../data/Small/Reservoirs_Madeira.csv",
                           "../data/Small/Stations_Madeira.csv",
                           "../data/Small/Cities_Madeira.csv",
                           "../data/Small/Pipes_Madeira.csv");
}   catch (std::exception& e)
    {
        std::cout<<"init error: "<<e.what()<< "\n";
        return 1;
    }
/*
    Algorithms::simpleEdmondsKarpThatDoesntDeleteSourceAndSink(graph);
    auto res1= Algorithms::CanShutDownReservoirOptimized(graph,"R_3");

    Algorithms::simpleEdmondsKarp(graph);
    Algorithms::SetWaterIn(graph);
    auto res2= Algorithms::CanShutDownReservoir(graph,"R_3");


    for (auto r1:res1)
    {
        for (auto r2:res2)
        {
            if (r1.cityCode==r2.cityCode&&r1.waterLoss!=r2.waterLoss)
            {
                std::cout<<"Difference found: "<<r1.cityCode<<", brute force: "<<r2.waterLoss<<", optimized: "<<r1.waterLoss<<"\n";
            }
        }
    }
*/

    Algorithms::AddSourceAndSink(graph);

    Algorithms::simpleEdmondsKarpThatDoesntDeleteSourceAndSink(graph);
    Algorithms::SetWaterIn(graph);
    auto res1=Algorithms::CanDeletePumpingStationOptimized(graph,"PS_2");


    Algorithms::simpleEdmondsKarpThatDoesntDeleteSourceAndSink(graph);
    Algorithms::SetWaterIn(graph);
    auto res2 = Algorithms::CanDeletePumpingStationFrom0(graph, "PS_2");

    for (auto r1:res1)
    {
        for (auto r2:res2)
        {
            if (r1.cityCode==r2.cityCode)
            {
                std::cout<<"Difference found: "<<r1.cityCode<<", brute force: "<<r2.waterLoss<<", optimized: "<<r1.waterLoss<<"\n";
            }
        }
    }

    return 1;

    Menu::print("Welcome to the Water Management Program.");
    Menu::displayoptions();
    int c = true;
    while(c){
        int i = Menu::chooseoption();
        switch (i) {
            case 0:
                c = false;
                break;
            case 1:
                Menu::displayoptions();
                break;
            case 2:
                Algorithms::simpleEdmondsKarp(graph);
                Menu::printInfoCities(graph, Menu::getInput("Select your city or write NULL to select everything."));
                break;
            case 3:
                Menu::print("This will only work if you have run option 2");
                Menu::printCities(Algorithms::CitiesWithNotEnoughWater(graph), "These are the cities with not enough water");
                break;
            case 4:
                Algorithms::BalanceTheLoad(graph);
                break;


            default:
                c = false;
                break;
        }
    }

    Menu::print("Thanks for using our program. This was developed by Filipe Correia, Gabriela Silva e Gonçalo Nunes.");

    return 0;
}
