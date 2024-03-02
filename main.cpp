#include <iostream>
#include "Menu.h"
#include "graph/Graph.h"


int main() {
    std::cout << "Hello, World!" << std::endl;
    Menu::print("hello");
    Menu::displayoptions();

    Graph g;
    return 0;
}
