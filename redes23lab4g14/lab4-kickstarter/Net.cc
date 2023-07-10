#ifndef NET
#define NET

#include <string.h>
#include <omnetpp.h>

#include <list>
#include "packet_m.h"

using namespace omnetpp;
using namespace std;

class Net: public cSimpleModule {
private:
    cOutVector source;
    cOutVector distance;
    int nodeCount;

    struct NeighborData {
        int neighbor;
        int cost;
        int outGate;
        // Sobrecarga del operador < (para solucionar
        // error que salta a la hora de usar set)
        bool operator<(const NeighborData& other) const {
            // Aquí debes definir la lógica de comparación
            // basada en tus necesidades
            if (neighbor < other.neighbor)
                return true;
            else if (neighbor == other.neighbor) {
                if (cost < other.cost)
                    return true;
                else if (cost == other.cost)
                    return outGate < other.outGate;
            }
            return false;
        }
    };

    // avisa si ya se completó el mapeo
    bool mappingCompleted;

    // Lista de vecinos (usado para ser recorrida en la construccion de rtable)
    list<int> neighbors;

    // lista donde guarda todos los mensajes de nodos llegados por primera vez
    list<int> prevBroadcastedPackets;

    // mappingData contains a list of neighbors and the cost
    // for each neighbor
    set<NeighborData> mappingData;

    // rtable is built by all the data collected from mappingData
    map<int, int> rtable;

    // amount of node conections
    int numInterfaces;

    // Functions
    void sendMappingPkt(Packet *pkt);
    void sendAppPkt(Packet *pkt);
    void createRTable(void);

public:
    Net();
    virtual ~Net();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Net);

#endif /* NET */

Net::Net() {
}

Net::~Net() {
}

void Net::initialize() {
    source.setName("Source");
    distance.setName("Distance");

    // obtiene el numero de compuertas
    numInterfaces = gateSize("toLnk");

    mappingCompleted = false;
    nodeCount = 0;

    for (int out=0; out<numInterfaces; out++) {
        // Por cada salida que tenga el nodo saldra un
        // mapping packet
        Packet *mappingPkt = new Packet();
        mappingPkt->setKind(2);
        mappingPkt->setByteLength(20);
        mappingPkt->setHopCount(0);
        mappingPkt->setSource(this->getParentModule()->getIndex());
        mappingPkt->setDestination(this->getParentModule()->getIndex());
        mappingPkt->setOut(out);
        mappingPkt->setHopCount(mappingPkt->getHopCount() + 1);
        send(mappingPkt, "toLnk$o", out);

    }
}

void Net::finish() {
}

void Net::sendMappingPkt(Packet *pkt) {
    // Check if the packet has been previously broadcasted
    for (const auto& sender : prevBroadcastedPackets) {
        if (sender == pkt->getSource()) {
            //list<list<NeighborData>>::iterator it = mappingData.begin();
            if(pkt->getSource() != this->getParentModule()->getIndex()) {
                NeighborData nd;
                nd.neighbor = pkt->getSource();
                nd.cost = pkt->getHopCount();
                cGate *arrivalGate = pkt->getArrivalGate();
                nd.outGate = arrivalGate->getIndex();
                mappingData.insert(nd);
            }

            // Packet already broadcasted, discard it
            delete pkt;

            return;
        }
    }
    // Packet not broadcasted, not discard it
    prevBroadcastedPackets.push_back(pkt->getSource());

    for (int out=0; out<numInterfaces; out++) {
        NeighborData nd;
        nd.neighbor = pkt->getSource();
        nd.cost = pkt->getHopCount();
        cGate *arrivalGate = pkt->getArrivalGate();
        nd.outGate = arrivalGate->getIndex();
        mappingData.insert(nd);

        bool addNewNeighbor = true;
        for (const auto& n : neighbors) {
            if (nd.neighbor == n) {
                addNewNeighbor = false;
            }
        }
        if (addNewNeighbor) {
            neighbors.push_back(nd.neighbor);
        }

        Packet *newPkt = pkt->dup();
        newPkt->setOut(out);
        newPkt->setHopCount(pkt->getHopCount() + 1);
        send(newPkt, "toLnk$o", out);

    }

    delete pkt;
}

void Net::sendAppPkt(Packet *pkt) {
    pkt->setHopCount(pkt->getHopCount() + 1);

    int destination = pkt->getDestination();

    // Finds the gate out to destination to send the pkt
    auto it = rtable.find(destination);
    if (it != rtable.end()) {
        pkt->setOut(it->second);
    }

    send(pkt, "toLnk$o", pkt->getOut());
}

void Net::createRTable() {
    int minCost;
    int bestOutGate =0;
    for (const auto& n : neighbors) {
        minCost = 1000;
        for (const auto& elem : mappingData) {
            if (n == elem.neighbor) {
                if (minCost > elem.cost) {
                    minCost = elem.cost;
                    bestOutGate = elem.outGate;
                }
            }
        }
        rtable[n] = bestOutGate;
    }
    cout << endl << "rtable en nodo: " << this->getParentModule()->getIndex() << endl;
    for (const auto& pair : rtable) {
        std::cout << "Clave: " << pair.first << ", Valor: " << pair.second << std::endl;
    }
}

void Net::handleMessage(cMessage *msg) {

    // All msg (events) on net are packets
    Packet *pkt = (Packet *) msg;

    if (pkt->getDestination() == this->getParentModule()->getIndex()
        && pkt->getKind() == 2 && pkt->getHopCount() > 0) {
        // si es el paquete que llega es del mapeo
        // y es el que mando este mismo nodo,
        // actualizo las variables privadas
        nodeCount = pkt->getHopCount();

        delete pkt;

        //createRTable();
    }

    // If this node is the final destination, send to App
    else if (pkt->getDestination() == this->getParentModule()->getIndex()
        && pkt->getKind() != 2) {
        send(msg, "toApp$o");

        // Save the hops data
        source.record(pkt->getSource());
        distance.record(pkt->getHopCount());
    }
    // If not, forward the packet to some else... to who?
    else {
        if (pkt->getKind() == 2 && pkt->getHopCount() > 0) { // Es un mensaje de mapeo
            sendMappingPkt(pkt);
        }
        else { // Es un mensaje de la app
            if (!mappingCompleted) {
                createRTable();
                mappingCompleted = true;
                EV << "rtable creada y lista para su uso para el nodo: " << this->getParentModule()->getIndex() << endl;
            }

            sendAppPkt(pkt);
        }
    }
}
