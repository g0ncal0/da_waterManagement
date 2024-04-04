#include <iostream>
#include "Menu.h"
#include "graph/Graph.h"
#include "Parser.h"
#include "Algorithms/Algorithms.h"
using namespace std;

int main() {


    int m = Menu::getNumber("Choose model to work on. MADEIRA: 0; PT: 1 ");
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
                Algorithms::ChooseBalanceTheLoad(graph);
                break;
            case 5:
                Algorithms::shutDownReservoir(graph);
                break;
            case 6:
                Algorithms::deletePumpingStation(graph);
                break;
            case 7:
                Algorithms::criticalPipelines(graph);
                break;
            case 9:
                Menu::printEdges(graph, Menu::getInput("Do you want to choose a specific origin? Insert the code of origin or NULL."));
                break;

            default:
                c = false;
                break;
        }
    }

    Menu::print("Thanks for using our program. This was developed by Filipe Correia, Gabriela Silva and Gonçalo Nunes.");

    return 0;
}
