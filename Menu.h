//
// Created by Filipe Correia on 02/03/2024.
//

#ifndef WM_MENU_H
#define WM_MENU_H

#include <string>
#include <iostream>
#include <vector>

class Menu {
    public:
    static std::vector<std::string> getOptions();
    static void print(std::string t);
    static void printList(std::vector<std::string> v);
    static void displayoptions();
    static int getNumber();
    static int chooseoption();
};


#endif //WM_MENU_H
