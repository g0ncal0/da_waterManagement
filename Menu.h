//
// Created by Filipe Correia on 02/03/2024.
//

#ifndef WM_MENU_H
#define WM_MENU_H

#include <string>
#include <iostream>
#include <vector>

class Menu {
private:
    public:
    static std::vector<std::string> getOptions(){
        return std::vector<std::string>({"Max Amount of Water by city", "Are needs met?", "Balance the load", "[RELIABILITY] One water reservoir is out", "[RELIABILITY] Which pumping stations are essential", "[RELIABILITY] Which pipelines are essential by city?", "[RELIABILITY] What cities are affected by mal-functioning of a pipeline."});
    }
    static void print(const char* t){
        std::cout << t << "\n";
    }
    static void printList(std::vector<std::string> v){
        for(int i = 0; i < v.size(); i++){
            std::cout << i << " : " << v[i] << '\n';
        }
    }
    static void displayoptions(){
        print("Options:");
        printList(getOptions());
    }
    static int getNumber() {
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
    static int chooseoption(){
        int choosen = getOptions().size() + 1;
        while(choosen >= getOptions().size() || choosen < 0){
            choosen = getNumber();
        }
        return choosen;
    }
};


#endif //WM_MENU_H
