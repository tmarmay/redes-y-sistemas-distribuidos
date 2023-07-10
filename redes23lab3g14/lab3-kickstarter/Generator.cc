#ifndef GENERATOR
#define GENERATOR

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Generator : public cSimpleModule {
    private:
        cMessage *sendMsgEvent;
        cOutVector packetsSentVector;
        int pktsSent;

        void sendPkt();
    public:
        Generator();
        virtual ~Generator();
    protected:
        virtual void initialize();
        virtual void finish();
        virtual void handleMessage(cMessage *msg);
};
Define_Module(Generator);

Generator::Generator() {
    sendMsgEvent = NULL;

}

Generator::~Generator() {
    cancelAndDelete(sendMsgEvent);
}

void Generator::initialize() {
    // create the send packet
    sendMsgEvent = new cMessage("sendEvent");
    packetsSentVector.setName("sent packets");
    pktsSent = 0;
    // schedule the first event at random time
    scheduleAt(par("generationInterval"), sendMsgEvent);
}

void Generator::finish() {
}

void Generator::sendPkt() {
    // create new packet
    cPacket *pkt = new cPacket("packet");
    // configure packet size
    pkt->setByteLength(par("packetByteSize"));
    // send to the output
    send(pkt, "out");
}

void Generator::handleMessage(cMessage *msg) {
    // sends a new packet
    sendPkt();

    // stats
    pktsSent++;
    packetsSentVector.record(pktsSent);

    // compute the new departure time, considering generationInterval
    simtime_t departureTime = simTime() + par("generationInterval");
    // schedule the new packet generation
    scheduleAt(departureTime, sendMsgEvent);
}

#endif /* GENERATOR */
