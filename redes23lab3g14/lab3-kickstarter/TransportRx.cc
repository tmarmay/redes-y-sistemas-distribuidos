#ifndef TRANSPORTRX
#define TRANSPORTRX

#include <string.h>
#include <omnetpp.h>

#include "FeedbackPkt_m.h"

using namespace omnetpp;

class TransportRx : public cSimpleModule {
    private:
        cQueue buffer;
        cQueue bufferFeedback;
        cMessage *endServiceEvent;
        cMessage *feedbackEvent;
        simtime_t serviceTime;
        cOutVector bufferSizeVector;
        cOutVector packetDropVector;
        int pktsDropped;

        void sendPkt();
        void sendFeedbackPkt();
        void enqueueMsg(cMessage *msg);
        void enqueueFeedbackMsg(cMessage *msg);
    public:
        TransportRx();
        virtual ~TransportRx();
    protected:
        virtual void initialize();
        virtual void finish();
        virtual void handleMessage(cMessage *msg);
};

Define_Module(TransportRx);

TransportRx::TransportRx() {
    endServiceEvent = NULL;
    feedbackEvent = NULL;
}

TransportRx::~TransportRx() {
    cancelAndDelete(endServiceEvent);
    cancelAndDelete(feedbackEvent);
}

void TransportRx::initialize() {
    buffer.setName("buffer");
    bufferFeedback.setName("receptor feedback status buffer");
    endServiceEvent = new cMessage("endService");
    feedbackEvent = new cMessage("endFeedback");
    bufferSizeVector.setName("buffer size");
    packetDropVector.setName("packet drop");
    pktsDropped = 0;
}

void TransportRx::finish() {
    recordScalar("Dropped packets", pktsDropped);
}

void TransportRx::sendPkt() {
    // if packet in buffer, send it
    if (!buffer.isEmpty()) {
        // dequeue packet
        cPacket *pkt = (cPacket*) buffer.pop();
        // send packet
        send(pkt, "toApp");
        // add packet processing duration
        simtime_t serviceTime = pkt->getDuration();
        // start new service
        scheduleAt(simTime() + serviceTime, endServiceEvent);
    }
}

void TransportRx::sendFeedbackPkt() {
    if (!bufferFeedback.isEmpty()){
        // dequeue packet
        FeedbackPkt *fdpkt = (FeedbackPkt *)bufferFeedback.pop();
        // send packet
        send(fdpkt, "toOut$o");
        scheduleAt(simTime() + fdpkt->getDuration(), feedbackEvent);
    }
}

void TransportRx::enqueueMsg(cMessage *msg) {
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
            FeedbackPkt *fbPkt = new FeedbackPkt();
            fbPkt->setByteLength(20);
            fbPkt->setKind(2);
            if (buffer.getLength() >= bufferMaxUsage){
                // manda feedback para que baje la velocidad de envio
                fbPkt->setFullBufferRx(true);
                this->bubble("feedback (nearly full buffer)");
            } else {
                // manda feedback para que aumente la velocidad de envio
                fbPkt->setSpeedUpTransmition(true);
                this->bubble("feedback (speed up)");
            }

            //enqueues feedback msg
            bufferFeedback.insert(fbPkt);
            if (!feedbackEvent->isScheduled()) {
                // If there are no msg sent, send this one
                scheduleAt(simTime(), feedbackEvent);
            }
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

void TransportRx::enqueueFeedbackMsg(cMessage *msg) {
    //enqueues feedback msg
    bufferFeedback.insert(msg);

    if (!feedbackEvent->isScheduled()) {
        // If there are no msg sent, send this one
        scheduleAt(simTime(), feedbackEvent);
    }
}

void TransportRx::handleMessage(cMessage *msg) {
    bufferSizeVector.record(buffer.getLength());

    if (msg->getKind() == 2){
        //if msg is feedback
        enqueueFeedbackMsg(msg);
    } else {
        if (msg == endServiceEvent) {
            // if msg is signaling an endServiceEvent
            sendPkt();
        } else if (msg == feedbackEvent) {
            // if msg is a feedback
            sendFeedbackPkt();
        } else {
            // if msg is a data packet
            enqueueMsg(msg);
        }
    }
}

#endif /* TRANSPORTRX */
