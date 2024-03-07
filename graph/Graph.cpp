#include "Graph.h"

///*******************///
/* GRAPH */

Graph* Graph::clone() const
{
    Graph* graph= new Graph();

    for (Vertex* vert:getVertexSet())
    {
        graph->addVertex(vert->clone());
    }
    for (Vertex* vert:getVertexSet()) {
        for(Edge* edge: vert->getAdj())
        {
            graph->addEdge(edge->getOrig()->getCode(),edge->getDest()->getCode(),edge->getCapacity());
        }

    }
//this assumes that there aren't any two edges with the same destination and the same origin.
    for (Vertex* vert:getVertexSet()) {
        for(Edge* edge: vert->getAdj())
        {
            if(edge->getReverse()!= nullptr)
            {
                Vertex* orig= graph->findVertex(edge->getOrig()->getCode());
                Vertex* dest= graph->findVertex(edge->getDest()->getCode());
                Edge* e1= nullptr;
                for (Edge* e:orig->getAdj())
                {
                    if(e->getDest()==dest)
                    {
                        e1=e;
                        break;
                    }
                }
                if(e1== nullptr){continue;}
                if(e1->getReverse()!=0)
                {
                    continue;
                }
                for (Edge* e:dest->getAdj())
                {
                    if(e->getDest()==orig)
                    {
                        e1->setReverse(e);
                        e->setReverse(e1);
                        break;
                    }
                }

            }
        }
    }
    return graph;
}
bool Graph::addVertex(Vertex* vert) {
    if(findVertex(vert->getCode()))
    {
        return false;
    }
    vertexSet.push_back(vert);
    return true;
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
    

    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it) == v) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u : vertexSet) {
                u->removeEdgeTo(v);
            }
            vertexSet.erase(it);
            delete v;
            return true;
        }
    }

    return false;
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
        Vertex* City::clone() const
        {
return new City(name,id,code,demand,population);

        }
    City::City(const std::string &name, int id, const std::string &code, int demand, int population) : Vertex(id,code){
        this->name = name;
        this->id = id;
        this->demand = demand;
        this->population = population;
    }

    /*City::City() : Vertex() {
        this->name = "";
        this->id = 0;
        this->demand = 0;
        this->population = 0;
    }*/

    char City::getType() { return 'c';}

    std::string City::getName(){return name;}
    int City::getDemand(){return demand;}
    int City::getTotalWaterIn() const {
        return totalWaterIn;
    }
    void City::setTotalWaterIn(int totalWaterIn) {
        this->totalWaterIn = totalWaterIn;
    }

///_____RESERVOIR_____
Vertex* Reservoir::clone() const{
        return new Reservoir(name,municipality,id,code,delivery);

    }
    Reservoir::Reservoir(const std::string &name, const std::string &municipality, int id, const std::string &code, int delivery) : Vertex(id,code){
        this->name = name;
        this->municipality = municipality;
        this->delivery = delivery;
    }
    char Reservoir::getType() { return 'r';}

    int Reservoir::getDelivery() const {
        return delivery;
    }

    int Reservoir::getActualDelivery() const {
        return actualDelivery;
    }

    void Reservoir::setActualDelivery(int actualDelivery) {
        this->actualDelivery = actualDelivery;
    }
///___Station___
    Vertex* Station::clone() const
    {
        return new Station(id,code);
    }

    Station::Station(int id, const std::string &code) : Vertex(id, code){}
    char Station::getType() { return 's';}

///___VERTEX____

std::vector<Edge*> Vertex::getAdj(){return adj;}

Vertex::Vertex(int id,const std::string& code){
    this->id = id;
    this->code = code;
}

/*Vertex::Vertex() {
    this->id = 0;
    this->code = "";
}*/

int Vertex::getId() const{return id;}
const std::string& Vertex::getCode(){return code;}
bool Vertex::addOutgoingEdge(Edge* edge) {
    adj.push_back(edge);
    return true;
}

bool Vertex::addIncomingEdge(Edge* edge) {
    incoming.push_back(edge);
    return true;
}

bool Vertex::isVisited() const {
    return this->visited;
}

bool Vertex::isProcessing() const {
    return this->processing;
}

double Vertex::getDist() const {
    return this->dist;
}

Edge *Vertex::getPath() const {
    return this->path;
}

std::vector<Edge *> Vertex::getIncoming() const {
    return this->incoming;
}

void Vertex::setVisited(bool visited) {
    this->visited = visited;
}

void Vertex::setProcessing(bool processing) {
    this->processing = processing;
}

void Vertex::setDist(double dist) {
    this->dist = dist;
}

void Vertex::setPath(Edge *path) {
    this->path = path;
}

void Vertex::removeOutgoingEdges() 
{
    for (Edge* edge:adj)
    {
        delete edge;
    }
    adj.resize(0);
}

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
