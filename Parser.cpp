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

    auto getUntilComma=[]( istringstream& data, std::string& toSet){
    
    ostringstream helper;
    char c;
    do
    {
    data >> c;
    }while (c==',');
    
    while (c!=',')
    {
       helper << c; 
    data >> c;
    }
    toSet = helper.str();
    helper.clear();
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
    reservoirData>>id;
    getUntilComma(reservoirData,code);
    reservoirData>>maxDelivery;
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
    istringstream stationData(line);


    string code;
    long id;


    stationData>>id;
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



{
ifstream cityFile(cityFilePath);
getline(cityFile,line); //The first line doesn't contain valid information
while (getline(cityFile,line))
{
    istringstream cityData(line);


    string name,code;
    long id,population;
    double demand;

    getUntilComma(cityData,code);
    cityData>>id;
    getUntilComma(cityData,code);
    cityData>>demand;
    cityData>>population;


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
    //else
    {
        cout<<"Unexpected line found in "<<pipeFilePath<<":"<<line<<"\n";
    }
    
}
}





return 0;
}
