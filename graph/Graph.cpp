#include "Graph.h"

///*******************///
/* GRAPH */

bool Vertex::removeEdgeTo(Vertex *d) {
    // HINT: use an iterator to scan the "adj" vector and then erase the edge.
    for (auto it = adj.begin(); it != adj.end(); it++) {
        if ((*it)->getDest() == d) {
            adj.erase(it);
            return true;
        }
    }
    return false;
}

Vertex* Graph::findVertex(const std::string& code)
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

bool Graph::removeVertex(Vertex* v) {
    auto iterator = findVertex(v->getCode());
    if(iterator == NULL){
        return false;
    }

    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it) == v) {
            (*it)->adj.clear();
            it = vertexSet.erase(it);
            it--;
        }
        else {
            (*it)->removeEdgeTo(v);
        }
    }

    return true;
}

    std::vector<Vertex*> Graph::getVertexSet() const{return vertexSet;}




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


Vertex* Graph::addCity(std::string &name, int id, std::string &code, int demand, int population){
    City* city = new City(name,id,code,demand, population);
    vertexSet.push_back(city);
    return city;
}

Vertex* Graph::addReservoir(std::string &name, std::string &municipality, int id, std::string &code, int delivery){
    Reservoir* reservoir = new Reservoir(name, municipality,id, code,delivery);
    vertexSet.push_back(reservoir);
    return reservoir;
}

Vertex* Graph::addStation(int id, std::string &code){
    Station* station = new Station(id, code);
    vertexSet.push_back(station);
    return station;
}


    bool Graph::addEdge(const std::string& origin,const std::string& target, double capacity )
    {
        auto firstVertex=findVertex(origin);
        auto secondVertex=findVertex(target);
        if (secondVertex==NULL||firstVertex==NULL)
        {
            return 1;
        }
        auto edge= new Edge(firstVertex,secondVertex,capacity);
        firstVertex->addOutgoingEdge(edge);
        secondVertex->addIncomingEdge(edge);
        
return 0;
    }
    //this can't be here because it need to call a function from the vertex which isn't defined yet...
    bool Graph::addBidirectionalEdge(const std::string& vertexA,const std::string& vertexB, double capacity){
        auto firstVertex=findVertex(vertexA);
        auto secondVertex=findVertex(vertexB);
        if (secondVertex==NULL||firstVertex==NULL)
        {
            return 1;
        }

        auto edge1= new Edge(firstVertex,secondVertex,capacity);
        auto edge2= new Edge(secondVertex,firstVertex,capacity);
        edge1->setReverse(edge2);
        edge2->setReverse(edge1); 
        firstVertex->addOutgoingEdge(edge1);
        secondVertex->addIncomingEdge(edge1);
        firstVertex->addIncomingEdge(edge2);
        secondVertex->addOutgoingEdge(edge2);
return 0;
    }



///_____EDGE_____
    Edge::Edge(Vertex *orig, Vertex *dest, double capacity){
        this->orig = orig;
        this->dest = dest;
        this->capacity = capacity;
    }

    Vertex * Edge::getDest() const{return dest;}
    double Edge::getCapacity() const{return capacity;}
    bool Edge::isSelected() const{return selected;}
    Vertex * Edge::getOrig() const{return orig;}
    Edge *Edge::getReverse() const{return reverse;}
    double Edge::getFlow() const{return flow;}

    void Edge::setSelected(bool selected){this->selected = selected;}
    void Edge::setReverse(Edge *reverse){this->reverse = reverse;}
    void Edge::setFlow(double flow){this->flow = flow;};
///____CITY_____

    City::City(std::string &name, int id, std::string &code, int demand, int population) : Vertex(id,code){
        this->name = name;
        this->id = id;
        this->demand = demand;
        this->population = population;
    }
    char City::getType() { return 'c';}

    std::string City::getName(){return name;}
    int City::getDemand(){return demand;}
///_____RESERVOIR_____
    Reservoir::Reservoir(std::string &name, std::string &municipality, int id, std::string &code, int delivery) : Vertex(id,code){
        this->name = name;
        this->municipality = municipality;
        this->delivery = delivery;
    }
    char Reservoir::getType() { return 'r';}

///___STATION

    Station::Station(int id, std::string &code) : Vertex(id,code){
        
    }
    char Station::getType() { return 's';}
///___VERTEX____
    std::vector<Edge*> Vertex::getAdj(){return adj;}
    Vertex::Vertex(int id,const std::string& code){this->id = id;
    this->code = code;
    }
    int Vertex::getId() const{return id;};
    const std::string& Vertex::getCode(){return code;}
    bool Vertex::addOutgoingEdge(Edge* edge)
    {
        adj.push_back(edge);
        return 0;
    }
    bool Vertex::addIncomingEdge(Edge* edge)
    {
        incoming.push_back(edge);
        return 0;
    }