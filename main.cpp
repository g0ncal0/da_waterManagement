#include <iostream>
#include "Menu.h"
#include "graph/Graph.h"
#include "Parser.h"
#include "Algorithms/Algorithms.h"
using namespace std;

int main() {


    int m = Menu::getNumber("Choose model to work on. MADEIRA: 0; PT: 1");
    Graph *graph;
    try {
        Parser p;
        if(m == 0){
            graph = p.parse("../data/Small/Reservoirs_Madeira.csv",
                            "../data/Small/Stations_Madeira.csv",
                            "../data/Small/Cities_Madeira.csv",
                            "../data/Small/Pipes_Madeira.csv");
        }else{
            graph = p.parse("../data/Large/Reservoirs.csv",
                            "../data/Large/Stations.csv",
                            "../data/Large/Cities.csv",
                            "../data/Large/Pipes.csv");
        }
    }catch (std::exception& e){
        std::cout<<"init error: "<<e.what()<< "\n";
        return 1;
    }






    Algorithms::AddSourceAndSink(graph);
    Algorithms::simpleEdmondsKarpThatDoesntDeleteSourceAndSink(graph);
    auto res1= Algorithms::CanShutDownReservoirOptimized(graph,"R_3");


    Algorithms::SetFlowToZero(graph);
    Algorithms::simpleEdmondsKarpThatDoesntDeleteSourceAndSink(graph);
    Algorithms::SetWaterIn(graph);
    auto res2= Algorithms::CanShutDownReservoir(graph,"R_3");


    for (auto& r1:res1)
    {
        for (auto& r2:res2)
        {
            if (r1.cityCode==r2.cityCode)
            {
                std::cout<<"Difference found: "<<r1.cityCode<<", brute force: "<<r2.waterLoss<<", optimized: "<<r1.waterLoss<<"\n";
            }
        }
    }

std::cout<<"First is done\n";




    //Algorithms::AddSourceAndSink(graph);
    Algorithms::SetFlowToZero(graph);
    Algorithms::simpleEdmondsKarpThatDoesntDeleteSourceAndSink(graph); //all my edmonds-karp are failing, for some reason...
    Algorithms::SetWaterIn(graph);
    auto res3=Algorithms::CanDeletePumpingStationOptimized(graph,"PS_2");


    Algorithms::SetFlowToZero(graph);
    Algorithms::simpleEdmondsKarpThatDoesntDeleteSourceAndSink(graph);
    Algorithms::SetWaterIn(graph);
    auto res4 = Algorithms::CanDeletePumpingStationFrom0(graph, "PS_2");

    //at least I know that it isn't related to the order in which the algorithms are called. It's really a problem with the ignore vertex function...

    for (auto& r1:res3)
    {
        for (auto& r2:res4)
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
            case 9:
                Menu::printEdges(graph, Menu::getInput("Do you want to choose a specific origin? Insert the code of origin or NULL."));
                break;
            default:
                c = false;
                break;
        }
    }

    Menu::print("Thanks for using our program. This was developed by Filipe Correia, Gabriela Silva e Gonçalo Nunes.");

    return 0;
}
