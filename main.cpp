#include <iostream>
#include "Menu.h"
#include "graph/Graph.h"


int main() {
    std::cout << "Hello, World!" << std::endl;
    Menu::print("hello");
    Menu::displayoptions();

    Graph g;
    g.addCity((std::string &) "Porto", 1, (std::string &) "C1", 10, 1000);
    g.findCity("Porto");
    return 0;
}
