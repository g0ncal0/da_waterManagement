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
