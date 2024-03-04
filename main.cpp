#include <iostream>
#include "Menu.h"
#include "graph/Graph.h"
using namespace std;
#include "Parser.h"
int main() {
  //  Menu::print("hello");
  //  Menu::displayoptions();


    Parser p;
    Graph* graph=p.parse("data/Small/Reservoirs_Madeira.csv", 
        "data/Small/Stations_Madeira.csv", 
        "data/Small/City_Madeira.csv",
        "data/Small/Pipes_Madeira.csv");

    std::cout << "Hello, World!" << std::endl;

    Graph g;
    string city = "Porto";
    string code = "C1";
    g.addCity(city, 1, code, 10, 1000);
    g.findCity("Porto");
    return 0;
}
