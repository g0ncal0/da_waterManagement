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

    Vertex* findVertex(const std::string& code);
    bool addEdge(const std::string& origin,const std::string& target, double capacity );
    bool addBidirectionalEdge(const std::string& vertexA,const std::string& vertexB, double capacity);
    bool removeVertex(Vertex* v);
    std::vector<Vertex*> getVertexSet() const;
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
    std::vector<Edge*> getAdj();
    Vertex(int id,const std::string& code);
    int getId() const;
    virtual char getType()=0;
    const std::string& getCode();
    bool addOutgoingEdge(Edge* edge);
    bool addIncomingEdge(Edge* edge);
    bool removeEdgeTo(Vertex *d);

    friend class Graph;
};

class Station : public Vertex{

public:
    Station(int id, std::string &code);
    char getType() override;

};

class Reservoir : public Vertex{
private:
    std::string name;
    std::string municipality;
    int delivery;
public:
    Reservoir(std::string &name, std::string &municipality, int id, std::string &code, int delivery);
    char getType() override;
};


class City : public Vertex{

private:
    std::string name;
    int id;
    int demand;
    int population;
public:
    City(std::string &name, int id, std::string &code, int demand, int population);
    char getType() override;

    std::string getName();
    int getDemand();

};



class Edge{
protected:
    Vertex * dest; // destination vertex
    double capacity=0; // edge weight, can also be used for capacity

    // auxiliary fields
    bool selected = false;

    // used for bidirectional edges
    Vertex *orig;
    Edge *reverse = nullptr;

    double flow; // for flow-related problems
public:
    Edge(Vertex *orig, Vertex *dest, double capacity);

    Vertex * getDest() const;
    double getCapacity() const;
    bool isSelected() const;
    Vertex * getOrig() const;
    Edge *getReverse() const;
    double getFlow() const;

    void setSelected(bool selected);
    void setReverse(Edge *reverse);
    void setFlow(double flow);


};







#endif //WM_GRAPH_H
