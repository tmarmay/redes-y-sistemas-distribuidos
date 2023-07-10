#ifndef QUEUE
#define QUEUE

#include <string.h>
#include <omnetpp.h>

#include "FeedbackPkt_m.h"

using namespace omnetpp;

class Queue: public cSimpleModule {
    private:
        cQueue buffer;
        cMessage *endServiceEvent;
        simtime_t serviceTime;
        cOutVector bufferSizeVector;
        cOutVector packetDropVector;
        int pktsDropped;

        void sendPkt();
        void enqueueMsg(cMessage *msg);
    public:
        Queue();
        virtual ~Queue();
    protected:
        virtual void initialize();
        virtual void finish();
        virtual void handleMessage(cMessage *msg);
};

Define_Module(Queue);

Queue::Queue() {
    endServiceEvent = NULL;
}

Queue::~Queue() {
    cancelAndDelete(endServiceEvent);
}

void Queue::initialize() {
    buffer.setName("buffer");
    endServiceEvent = new cMessage("endService");
    bufferSizeVector.setName("buffer size");
    packetDropVector.setName("packet drop");
    pktsDropped = 0;

}

void Queue::finish() {
}

void Queue::sendPkt() {
    // if packet in buffer, send next one
    if (!buffer.isEmpty()) {
        // dequeue packet
        cPacket *pkt = (cPacket*) buffer.pop();
        // send packet
        send(pkt, "out");
        // add packet processing duration
        serviceTime = pkt->getDuration();
        // start new service
        scheduleAt(simTime() + serviceTime, endServiceEvent);
    }
}

void Queue::enqueueMsg(cMessage *msg) {
    const int bufferMaxSize = (int)par("bufferSize");
    // Ponemos como limite de uso del buffer del 90%
    const int bufferMaxUsage = 0.9 * bufferMaxSize;
    // controlar que no reduzca al maximo la velocidad de envio
    const int bufferMinUsage = 0.5 * bufferMaxSize;
    //check the buffer limit
    if (buffer.getLength() >= bufferMaxSize){
        //drop the packet
        delete msg;
        // show message in simulation
        this->bubble("packet dropped");
        pktsDropped++;
        packetDropVector.record(pktsDropped);
    } else {
        if (buffer.getLength() >= bufferMaxUsage
            || buffer.getLength() > bufferMinUsage){
            // si el tamaño del buffer se pasa de lo esperado
            // o el tamaño bajó mas de lo esperado genera
            // feedback
            FeedbackPkt *fbPkt = new FeedbackPkt();
            fbPkt->setByteLength(20);
            fbPkt->setKind(2);
            if (buffer.getLength() >= bufferMaxUsage){
                // manda feedback para que baje la velocidad de envio
                fbPkt->setFullBufferQueue(true);
                this->bubble("feedback (nearly full buffer)");
            } else {
                // manda feedback para que aumente la velocidad de envio
                fbPkt->setSpeedUpTransmition(true);
                this->bubble("feedback (speed up)");
            }
            buffer.insertBefore(buffer.front(), fbPkt);
        }
        // enqueue the packet
        buffer.insert(msg);
        // if the server is idle
        if (!endServiceEvent->isScheduled()) {
            // start the service
            scheduleAt(simTime(), endServiceEvent);
        }
    }
}

void Queue::handleMessage(cMessage *msg) {
    // stats
    bufferSizeVector.record(buffer.getLength());

    if (msg == endServiceEvent) {
        // if msg is signaling an endServiceEvent
        sendPkt();
    } else {
        // if msg is a data packet
        enqueueMsg(msg);

    }
}

#endif /* QUEUE */
