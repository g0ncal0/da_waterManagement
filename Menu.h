//
// Created by Filipe Correia on 02/03/2024.
//

#ifndef WM_MENU_H
#define WM_MENU_H

#include <string>
#include <iostream>
#include <vector>
#include "graph/Graph.h"

class Menu {
    public:
    static std::vector<std::string> getOptions();
    static void print(std::string t);
    static void printList(std::vector<std::string> v);
    static void displayoptions();
    static int getNumber(std::string text);
    static int chooseoption();
    static void printStatistics(float avg, int max, float variance, int nedges);
    static void printInfoCities(Graph* g, std::string city);
    static std::string getInput(std::string text);
    static void printCities(std::vector<City*> v, std::string text = "");

};


#endif //WM_MENU_H
