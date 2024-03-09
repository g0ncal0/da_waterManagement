//
// Created by Filipe Correia on 02/03/2024.
//

#include "Menu.h"

#include <iostream>
     std::vector<std::string> Menu::getOptions(){
    return {"Max Amount of Water by city", "Are needs met?", "Balance the load", "[RELIABILITY] One water reservoir is out", "[RELIABILITY] Which pumping stations are essential", "[RELIABILITY] Which pipelines are essential by city?", "[RELIABILITY] What cities are affected by mal-functioning of a pipeline."};
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
    int Menu::getNumber() {
        int n = -1;
        while (n == -1) {
            try {
                std::cin >> n;
            } catch (...) {
                std::cout << "error. try again";
            }
        }
        return n;
    }
    int Menu::chooseoption(){
        int choosen = 6 + 1;
        while(choosen >= 7  || choosen < 0){
            choosen = getNumber();
        }
        return choosen;
    }