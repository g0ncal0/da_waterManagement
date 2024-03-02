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
    bool addCity(std::string &name, int id, std::string &code, int demand, int population);
    bool addReservoir(std::string &name, std::string &municipality, int id, std::string &code, int delivery);
    bool addStation(int id, std::string &code);

    Vertex* findVertex(const std::string& code)
    {
        for (size_t i = 0; i <vertexSet.size(); i++)
        {
            if (vertexSet[i]->getCode() == code)
            {
                return vertexSet[i];
            }
            
        }
        return NULL;
    }
    bool addEdge(const std::string& origin,const std::string& target, double capacity )
    {
        auto firstVertex=findVertex(origin);
        auto secondVertex=findVertex(target);
        if (secondVertex==NULL||firstVertex==NULL)
        {
            return 1;
        }


        

    }
    bool addBidirectionalEdge(const std::string& vertexA,const std::string& vertexB, double capacity){
        auto firstVertex=findVertex(vertexA);
        auto secondVertex=findVertex(vertexB);
        if (secondVertex==NULL||firstVertex==NULL)
        {
            return 1;
        }



    }

    bool removeVertex(Vertex* v){
        auto it = std::find(vertexSet.begin(), vertexSet.end(), v);
        if(it == vertexSet.end()){
            return false;
        }
        vertexSet.erase(it);
        return true;
    }

    std::vector<Vertex*> getVertexSet() const{return vertexSet;}


};


class Vertex{
protected:
    std::vector<Edge *> adj;
    int id;
    bool visited = false;
    bool processing = false;
    double dist = 0;
    Edge *path = nullptr;
    std::vector<Edge *> incoming;
    std::string code;

public:
    std::vector<Edge*> getAdj(){return adj;}
    Vertex(int id,const std::string& code){this->id = id;
    this->code = code;
    }
    int getId() const{return id;};
    virtual char getType()=0;
    const std::string& getCode(){return code;}

};

class Station : public Vertex{

public:
    Station(int id, std::string &code) : Vertex(id,code){
        
    }
    char getType() override{ return 's';}

};

class Reservoir : public Vertex{
private:
    std::string name;
    std::string municipality;
    int delivery;
public:
    Reservoir(std::string &name, std::string &municipality, int id, std::string &code, int delivery) : Vertex(id,code){
        this->name = name;
        this->municipality = municipality;
        this->delivery = delivery;
    }
    char getType() override{ return 'r';}
};


class City : public Vertex{

private:
    std::string name;
    int id;
    int demand;
    int population;
public:
    City(std::string &name, int id, std::string &code, int demand, int population) : Vertex(id,code){
        this->name = name;
        this->id = id;
        this->demand = demand;
        this->population = population;
    }
    char getType() override{ return 'c';}

    std::string getName(){return name;}
    int getDemand(){return demand;}

};



class Edge{
protected:
    Vertex * dest; // destination vertex
    double capacity; // edge weight, can also be used for capacity

    // auxiliary fields
    bool selected = false;

    // used for bidirectional edges
    Vertex *orig;
    Edge *reverse = nullptr;

    double flow; // for flow-related problems
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


bool Graph::addCity(std::string &name, int id, std::string &code, int demand, int population){
    City* city = new City(name,id,code,demand, population);
    vertexSet.push_back(city);
    return true;
}

bool Graph::addReservoir(std::string &name, std::string &municipality, int id, std::string &code, int delivery){
    Reservoir* reservoir = new Reservoir(name, municipality,id, code,delivery);
    vertexSet.push_back(reservoir);
    return true;
}

bool Graph::addStation(int id, std::string &code){
    Station* station = new Station(id, code);
    vertexSet.push_back(station);
    return true;
}

#endif //WM_GRAPH_H
