//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


#include "AbstractWRS.h"

namespace wrs {

Useful * Useful::_instance = 0;

AbstractWRS::AbstractWRS()
{
    msgServiced = endServiceMsg = NULL;
}

AbstractWRS::~AbstractWRS()
{
    delete msgServiced;
    cancelAndDelete(endServiceMsg);
}

void AbstractWRS::initialize()
{
    endServiceMsg = new cMessage("end-service");
    queue.setName("queue");
    qlenSignal = registerSignal("qlen");
    busySignal = registerSignal("busy");
    queueingTimeSignal = registerSignal("queueingTime");
    emit(qlenSignal, queue.length());
    emit(busySignal, 0);

	// store pointer to sink 7
	//s7 = check_and_cast<WRSink *>( getParentModule()->findObject("sink7", true) );

} // initialize()

void AbstractWRS::handleMessage(cMessage *msg)
{
    if (msg==endServiceMsg) {
        endService( msgServiced );
        if (queue.empty()) {
            msgServiced = NULL;
            emit(busySignal, 0);
        } else {
#if 1
            msgServiced = (cMessage *) queue.pop();
            emit(qlenSignal, queue.length());
            emit(queueingTimeSignal, simTime() - msgServiced->getTimestamp());
            simtime_t serviceTime = startService( msgServiced );
            scheduleAt( simTime()+serviceTime, endServiceMsg );

#endif
        }
    } else if (!msgServiced) {
        arrival( msg );
        msgServiced = msg;
        emit(queueingTimeSignal, 0.0);
        simtime_t serviceTime = startService( msgServiced );
        scheduleAt( simTime()+serviceTime, endServiceMsg );
        emit(busySignal, 1);
    } else {
        arrival( msg );
#if 0
        // determine priority from name
        int prio = Useful::getInstance()->getPriority( std::string(msg->getName()) );
        if( prio==7 ) {
        	// don't queue
			// sendDirect to port 7
			//std::cout << "Don't queue prio 7" << std::endl;
			sendDirect(msg, s7, "sendDirect");
        } else {
        	queue.insert( msg );
        	//std::cout << "!queued: " << msg->getName() << "    ql: " << queue.length() << std::endl;
        	msg->setTimestamp();
        	emit(qlenSignal, queue.length());
        }
#else
        queue.insert( msg );
		//std::cout << "!queued: " << msg->getName() << "    ql: " << queue.length() << std::endl;
		msg->setTimestamp();
		emit(qlenSignal, queue.length());
#endif
    }
} // handleMessage()


}; //namespace

