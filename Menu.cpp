//
// Created by Filipe Correia on 02/03/2024.
//

#include "Menu.h"

#include <iostream>
#include <sstream>




std::vector<std::string> Menu::getOptions(){
    return {"Exit Program", "Repeat Instructions", "Max Amount of Water by city (Execute Edmonds Karp)", "Are needs met?", "Balance the load", "[RELIABILITY] One water reservoir is out", "[RELIABILITY] Which pumping stations are essential", "[RELIABILITY] Which pipelines are essential by city?", "[RELIABILITY] What cities are affected by mal-functioning of a pipeline.", "[INFO] Print Info about edges"};
}

std::string Menu::getFileName(){
    return "../output.txt";
}


void Menu::print(std::string t){
    std::cout << t << "\n";
}
void Menu::printList(std::vector<std::string> v){
    for(int i = 0; i < v.size(); i++){
        std::cout << i << " : " << v[i] << "\n";
    }
}
void Menu::displayoptions(){
    print("Options:");
    printList(getOptions());
}
int Menu::getNumber(std::string text) {
    int n = -1;
    while (n == -1) {
        try {
            std::cout << text;
            std::cin >> n;
        } catch (...) {
            std::cout << "error. try again";
        }
    }
    return n;
}
int Menu::chooseoption(){
    int choosen = (int) Menu::getOptions().size() + 1;
    while(choosen >= Menu::getOptions().size()  || choosen < 0){
        choosen = getNumber("Your option: ");
        print("");
    }
    return choosen;
}

void Menu::printStatistics(float avg, int max, float variance, int nedges) {
    print("The statistics are the following:");
    std::cout << "Avg difference between edge capacity and flow: " << avg << "\nThe max difference: " << max << "\nThe variance: " << variance << "\nThis was calculated on " << nedges << " edges.\n";
}



void Menu::printInfoCities(Graph* g, std::string city){
    print("CITY   |  CODE  |  WATER ");
    std::stringstream fullstream;
    for(Vertex* v : g->getVertexSet()){
        if(v->getType() == 'c'){
            City* c = (City*) v;
            if(c->getName() == city){
                std::cout << c->getName() << " | " << c->getCode() << " | " << c->getTotalWaterIn() << "\n";
                return;
            }
            fullstream << c->getName() << " | " << c->getCode() << " | " << c->getTotalWaterIn() << "\n";
        }
    }
    print(fullstream.str());
    std::ofstream myfile;
    myfile.open (getFileName());
    myfile << fullstream.str();
    myfile.close();
}
std::string Menu::getInput(std::string text){
    print(text);
    std::string input;
    std::cin.clear();
    std::cin.ignore(1000, '\n');
    getline(std::cin, input);
    return input;
}

void Menu::printCities(std::vector<City*> v, std::string text){
    print(text);
    print("CITY NAME | CODE | WATER IN | DEMAND | DEFICIT");
    for(City* city : v){
        std::cout << city->getName() << " " << city->getCode() << " " << city->getTotalWaterIn() <<" " << city->getDemand() << " " << city->getDemand() - city->getTotalWaterIn() << "\n";
    }
}

void Menu::printEdges(Graph* g, std::string origin){
    print("ORIGIN | END | FLOW | CAPACITY ");
    std::stringstream fullstream;
    bool found = false;
    for(Vertex* v : g->getVertexSet()){
            for(Edge* e : v->getAdj()){
                if(v->getCode() == origin){
                    std::cout << e->getOrig()->getCode() << " | " << e->getDest()->getCode() << " | " << e->getFlow() << " | " << e->getCapacity() << "\n";
                    found = true;
                }
                fullstream << e->getOrig()->getCode() << " | " << e->getDest()->getCode() << " | " << e->getFlow() << " | " << e->getCapacity() << "\n";
            }
    }
    if(!found){
        print(fullstream.str());
    }
}



