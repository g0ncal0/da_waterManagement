#include <iostream>
#include "Menu.h"
#include "graph/Graph.h"


int main() {
    Menu::print("hello");
    Menu::displayoptions();
    //int option = Menu::chooseoption();

    Graph g;
    std::string n = "Porto";
    std::string c = "C1";
    g.addCity(n, 1, c, 10, 1000);
    Vertex* newcity = new City(n, 1, c, 10, 1000);
    std::cout << newcity->getType();

    std::cout << '\n' << g.findCity("Porto")->getType();
    return 0;
}
