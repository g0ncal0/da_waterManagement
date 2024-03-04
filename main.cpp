#include <iostream>
#include "Menu.h"
#include "graph/Graph.h"
using namespace std;

int main() {
    std::cout << "Hello, World!" << std::endl;
    Menu::print("hello");
    Menu::displayoptions();

    Graph g;
    string city = "Porto";
    string code = "C1";
    g.addCity(city, 1, code, 10, 1000);
    g.findCity("Porto");
    return 0;
}
