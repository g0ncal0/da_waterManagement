//
// Created by Filipe Correia on 02/03/2024.
//

#ifndef WM_GRAPH_H
#define WM_GRAPH_H

#include <vector>
#include <string>

class Vertex;
class City;
class Edge;

class Graph{
protected:
    std::vector<Vertex*> vertexSet;    // vertex set


public:
    Vertex *findCity(const std::string &name) const;
    Vertex *findCity(const int &id) const;
    Vertex *findReservoir(const int &id) const;
};


class Vertex{
public:


protected:
    std::vector<Edge *> adj;

    // auxiliary fields
    bool visited = false;
    bool processing = false;
    double dist = 0;
    Edge *path = nullptr;

    std::vector<Edge *> incoming; // incoming edges

};

class Station : public Vertex{
    Station(int id, std::string &code){
        this->id = id;
        this->code = code;
    }

protected:
    int id;
    std::string code;
};

class Reservoir : public Vertex{
public:
    Reservoir(std::string &name, std::string &municipality, std::string &code, int delivery){
        this->name = name;
        this->municipality = municipality;
        this->code = code;
        this->delivery = delivery;
    }

private:
    std::string name;
    std::string municipality;
    std::string code;
    int delivery;


};


class City : public Vertex{
public:
    City(std::string &name, int id, std::string &code, int demand, int population){
        this->name = name;
        this->id = id;
        this->code = code;
        this->demand = demand;
        this->population = population;
    }
    std::string getName(){return name;}
    int getId(){return id;}
    int getDemand(){return demand;}

protected:
    std::string name;
    int id;
    std::string code;
    int demand;
    int population;
};



class Edge{
public:
    Edge(Vertex *orig, Vertex *dest, double capacity){
        this->orig = orig;
        this->dest = dest;
        this->capacity = capacity;
    }

    Vertex * getDest() const{return dest;}
    double getCapacity() const{return capacity;}
    bool isSelected() const{return selected;}
    Vertex * getOrig() const{return orig;}
    Edge *getReverse() const{return reverse;}
    double getFlow() const{return flow;}

    void setSelected(bool selected){this->selected = selected;}
    void setReverse(Edge *reverse){this->reverse = reverse;}
    void setFlow(double flow){this->flow = flow;};

protected:
    Vertex * dest; // destination vertex
    double capacity; // edge weight, can also be used for capacity

    // auxiliary fields
    bool selected = false;

    // used for bidirectional edges
    Vertex *orig;
    Edge *reverse = nullptr;

    double flow; // for flow-related problems
};





///*******************///
/* GRAPH */

Vertex* Graph::findCity(const std::string &name) const{
    for(Vertex* v: vertexSet){
        City* c = dynamic_cast<City*>(v);
        if(c){
            // It's a city!
            if(c->getName() == name){
                return v;
            }
        }
    }
    return NULL;
}

Vertex* Graph::findCity(const int &id) const {
    for(Vertex* v: vertexSet){
        City* c = dynamic_cast<City*>(v);
        if(c){
            // It's a city!
            if(c->getId() == id){
                return v;
            }
        }
    }
    return NULL;
}





#endif //WM_GRAPH_H
