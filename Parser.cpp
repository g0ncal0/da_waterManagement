//
// Created by Filipe Correia on 02/03/2024.
//

#include "Parser.h"
#include "graph/Graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Graph* Parser::parse(const std::string& reservoirFilePath, const std::string& stationFilePath,
const std::string& cityFilePath, const std::string& pipeFilePath)
{

    auto getUntilComma=[]( istringstream& data, std::string& toSet,char end=',') {
    
        getline(data, toSet, end);
    };

    auto getLong=[](istringstream& data, long& toSet, char end = ','){

        std::string line;
        getline(data, line, end);
        if (line != "")
        {
            toSet = stol(line);
        }
    };
    auto getInt = [](istringstream& data, int& toSet, char end = ',') {
        std::string line;
        getline(data, line, end);
        if (line!="")
        {
            toSet = stoi(line);
        }
        };
    auto getDouble = [](istringstream& data, double& toSet, char end = ',') {
        std::string line;
        getline(data, line, end);
        if (line != "")
        {
            toSet = stod(line);
        }
        };

Graph* graph= new Graph();

string line; // to store the line currentl being read

//initializing reservoirs
{
ifstream reservoirFile(reservoirFilePath);
getline(reservoirFile,line); //The first line doesn't contain valid information
while (getline(reservoirFile,line))
{
    istringstream reservoirData(line);


    string name, municipality,code;
    long id,maxDelivery;


    getUntilComma(reservoirData,name);
    getUntilComma(reservoirData,municipality);   
    getLong(reservoirData,id);
    getUntilComma(reservoirData,code);
    getLong(reservoirData,maxDelivery);
    if (name!=""&&municipality!=""&&code!="")
    {
    graph->addReservoir(name,municipality,id,code,maxDelivery);
    }else
    {
        cout<<"Unexpected line found in "<<reservoirFilePath<<":"<<line<<"\n";
    }
    
}
}

//initializing stations
{

   

    ifstream stationFile(stationFilePath);
getline(stationFile,line); //The first line doesn't contain valid information
while (getline(stationFile,line))
{
    cout << "Iteration\n";
    istringstream stationData(line);


    string code;
    int id;

    getInt(stationData,id);
    getUntilComma(stationData,code);
    if (code!="")
    {
    graph->addStation(id,code);
    
    }else
    {
        cout<<"Unexpected line found in "<<stationFilePath<<":"<<line<<"\n";
    }
    
}

}

cout << "Escaped!!!\n";

{
ifstream cityFile(cityFilePath);
getline(cityFile,line); //The first line doesn't contain valid information
while (getline(cityFile,line))
{
    istringstream cityData(line);


    string name,code;
    int id; long population = 0;
    double demand;

    getUntilComma(cityData,name,',');
    getInt(cityData,id, ',');
    getUntilComma(cityData,code, ',');
    getDouble(cityData,demand);
    cityData.ignore(1);
    std::string helper,helper2;
    getUntilComma(cityData, helper,'\"');
    for (char c: helper) {
        if (c!=',')
        {
            helper2 += c;
        }
    }
    if (helper2!="")
    {
        population = stol(helper2);

    }

    if (code!=""&&name!="")
    {
        graph->addCity(name,id,code,demand,population);
    }else
    {
        cout<<"Unexpected line found in "<<cityFilePath<<":"<<line<<"\n";
    }
    
}
}





{
ifstream pipeFile(pipeFilePath);
getline(pipeFile,line); //The first line doesn't contain valid information
while (getline(pipeFile,line))
{
    istringstream pipeData(line);


    string codeOrigin, codeDestination;
    long capacity;
    bool isBi;

    getUntilComma(pipeData,codeOrigin);
    getUntilComma(pipeData,codeDestination);
    pipeData>>capacity;
    getLong(pipeData,capacity);
    pipeData>>isBi;


    if (codeOrigin!=""&&codeDestination!="")
    {
        if (isBi)
        {
            graph->addBidirectionalEdge(codeOrigin,codeDestination,capacity);
        }else
        {
         graph->addEdge(codeOrigin,codeDestination,capacity);    
        }
        
    }
    else
    {
        cout<<"Unexpected line found in "<<pipeFilePath<<":"<<line<<"\n";
    }
    
}
}





return graph;
}
