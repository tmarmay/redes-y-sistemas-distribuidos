#ifndef TRANSPORTTX
#define TRANSPORTTX

#include <string.h>
#include <omnetpp.h>

#include "FeedbackPkt_m.h"

using namespace omnetpp;

class TransportTx : public cSimpleModule {
    private:
        cQueue buffer;
        cMessage *endServiceEvent;
        double sendSpeed;
        cOutVector bufferSizeVector;

        void sendPkt();
        void enqueueMsg(cMessage *msg);
        void receiveFeedback(cMessage *msg);
    public:
        TransportTx();
        virtual ~TransportTx();
    protected:
        virtual void initialize();
        virtual void finish();
        virtual void handleMessage(cMessage *msg);
};

Define_Module(TransportTx);

TransportTx::TransportTx() {
    endServiceEvent = NULL;
}

TransportTx::~TransportTx() {
    cancelAndDelete(endServiceEvent);
}

void TransportTx::initialize() {
    buffer.setName("buffer");
    sendSpeed = 0.0;
    endServiceEvent = new cMessage("endService");
    bufferSizeVector.setName("buffer size");
}

void TransportTx::finish() {
}

void TransportTx::sendPkt() {
    // if packet in buffer, send next one
    if (!buffer.isEmpty()) {
        // dequeue packet
        cPacket *pkt = (cPacket*) buffer.pop();
        // send packet
        send(pkt, "toOut$o");
        // add packet processing duration
        simtime_t serviceTime = pkt->getDuration();
        serviceTime = (serviceTime + (serviceTime * sendSpeed));
        // start new service
        scheduleAt(simTime() + serviceTime, endServiceEvent);
     }
}

void TransportTx::enqueueMsg(cMessage *msg) {
    //check the buffer limit
    if (buffer.getLength() >= (int)par("bufferSize")){
        //drop the packet
        delete msg;
        // show message in simulation
        this->bubble("packet dropped");
    } else {
        // enqueue the packet
        buffer.insert(msg);
        // if the server is idle
        if (!endServiceEvent->isScheduled()) {
            // start the service
            scheduleAt(simTime(), endServiceEvent);
        }
    }
}

void TransportTx::receiveFeedback(cMessage *msg) {
    FeedbackPkt* fbPkt = (FeedbackPkt*)msg;

    if (fbPkt->getFullBufferQueue() || fbPkt->getFullBufferRx()) {
        // slow down send rate
        sendSpeed += 0.2;
    }
    if (fbPkt->getSpeedUpTransmition() && sendSpeed > 0) {
        // speed up send rate
        sendSpeed -= 0.1;
    }

    delete (msg);
}

void TransportTx::handleMessage(cMessage *msg) {
    bufferSizeVector.record(buffer.getLength());
    // msg is a packet
    if (msg->getKind() == 2) {
        // msg is a feedbackPkt
        receiveFeedback(msg);

    } else if (msg->getKind() == 0) {
        // msg is a data packet
        if (msg == endServiceEvent) {
            // if msg is signaling an endServiceEvent
            sendPkt();
        } else {
            // if msg is a data packet
            enqueueMsg(msg);
        }
    }
}

#endif /* TRANSPORTTX */
