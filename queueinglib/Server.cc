//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "Server.h"
#include "Job.h"
#include "SelectionStrategiesServer.h"

namespace queueing {

Define_Module(Server);

Server::Server()
{
    selectionStrategy = NULL;
    jobServiced = NULL;
    endServiceMsg = NULL;

    triggerServiceMsg = NULL;
}

Server::~Server()
{
    delete selectionStrategy;
    delete jobServiced;
    cancelAndDelete(endServiceMsg);

    cancelAndDelete(triggerServiceMsg);
}

void Server::initialize()
{
    busySignal = registerSignal("busy");
    emit(busySignal, 0);

    endServiceMsg = new cMessage("end-service");
    jobServiced = NULL;
    selectionStrategy = SelectionStrategyServer::create(par("fetchingAlgorithm"), this, true);
    if (!selectionStrategy)
        error("invalid selection strategy");


    // keep a pointer to queue 7
    _q7 = getQueue(7);

    // pointers to other queues
    for( int i=6; i>-1; i-- )
    	_qs.push_back( getQueue(i) );

    triggerServiceMsg = new cMessage("trigger");

    WATCH(numSent);
    numSent = 0;
}

void Server::handleMessage(cMessage *msg)
{
#if 1
	if( msg == triggerServiceMsg ) {
		//Job *job = check_and_cast<Job *>(msg);
		if( jobServiced!=NULL ) {
			send(jobServiced, "out");
			numSent++;
			std::cout << "server sent " << jobServiced->getName() << std::endl;
			jobServiced = NULL;
			emit(busySignal, 0);
		}
	} else {
		//std::cout << "server received " << msg->getName() << std::endl;
		if( strcmp(msg->getName(), "trigger")== 0 ) {
		// trigger test
			std::cout << "server triggered " << std::endl;


			// check queue lengths and request if length>0 as long as length>0
			if( _q7->length()>0 ) {
				std::cout << "q7 length " << _q7->length();
				while( _q7->length()>0 ) {
					_q7->request(0);
				}
			} else {
				// check all other queue gates
				for( int i=6; i>-1; i-- ) {
					if( _qs.at(i)->length()>0 ) {
						std::cout << "q" << i << " length " << _qs.at(i)->length();
						while( _qs.at(i)->length()>0 ) {
							_qs.at(i)->request(0);
						}
					}
				}
			}

		} else {

			if (jobServiced) {
				error("job arrived while already servicing one");
				std::cout << "job arrived while already servicing one" << std::endl;
			}

			jobServiced = check_and_cast<Job *>(msg);
			simtime_t serviceTime = par("serviceTime");
			scheduleAt(simTime()+serviceTime, triggerServiceMsg);
		}
	}
#else
#if 1
	// Original
    if (msg==endServiceMsg)
    {
    	//std::cout << "endServiceMsg " << endServiceMsg->getName() << std::endl;
        ASSERT(jobServiced!=NULL);
        simtime_t d = simTime() - endServiceMsg->getSendingTime();
        jobServiced->setTotalServiceTime(jobServiced->getTotalServiceTime() + d);
        send(jobServiced, "out");
        std::cout << jobServiced->getName() << " with prio " << jobServiced->getPriority() << " sent" << std::endl;
        numSent++;
        jobServiced = NULL;
        emit(busySignal, 0);

        if (ev.isGUI()) getDisplayString().setTagArg("i",1,"");

        // examine all input queues, and request a new job from a non empty queue
        int k = selectionStrategy->select();
        std::cout << "server selected gate " << k << std::endl;
        if (k >= 0)
        {
            EV << "requesting job from queue " << k << endl;
            std::cout << "requesting job from queue " << k << std::endl;
            cGate *gate = selectionStrategy->selectableGate(k);
            check_and_cast<IPassiveQueue *>(gate->getOwnerModule())->request(gate->getIndex());
        }
    }
    else
    {
        if (jobServiced) {
        	error("job arrived while already servicing one");
            std::cout << "job arrived while already servicing one" << std::endl;
        }

        jobServiced = check_and_cast<Job *>(msg);
        std::cout << "jobServiced " << jobServiced->getName() << " with prio " << jobServiced->getPriority() << " sent" << std::endl;
        simtime_t serviceTime = par("serviceTime");
        scheduleAt(simTime()+serviceTime, endServiceMsg);
        emit(busySignal, 1);

        if (ev.isGUI()) getDisplayString().setTagArg("i",1,"cyan");
    }
#else
	log("");
	//jobServiced = check_and_cast<Job *>(msg);
    if (msg==endServiceMsg)
    {
        ASSERT(jobServiced!=NULL);
        simtime_t d = simTime() - endServiceMsg->getSendingTime();
        jobServiced->setTotalServiceTime(jobServiced->getTotalServiceTime() + d);
        send(jobServiced, "out");
        std::cout << jobServiced->getName() << " with prio " << jobServiced->getPriority() << " sent" << std::endl;
        numSent++;
        jobServiced = NULL;
        emit(busySignal, 0);

        if (ev.isGUI()) getDisplayString().setTagArg("i",1,"");

        // examine all input queues, and request a new job from a non empty queue
        int k = selectionStrategy->select();
        /*if (k >= 0)
        //int k = 0;	// fixed	// TODO
        {
            EV << "requesting job from queue " << k << endl;
            std::cout << "requesting job from queue " << k << std::endl;
            cGate *gate = selectionStrategy->selectableGate(k);
            check_and_cast<IPassiveQueue *>(gate->getOwnerModule())->request(gate->getIndex());
        }*/

        std::cout << "k " << k << std::endl;
        if( _q7->length()>0 ) {
        	std::cout << "q7 length " << _q7->length();
        	_q7->request(7);
        } else {
			// check all other queue gates
			for( int i=6; i>-1; i-- ) {
				_qs.at(i)->request();
				/*cGate *gate = selectionStrategy->selectableGate(i);
				bool b = selectionStrategy->isSelectable(gate->getOwnerModule());
				std::cout << i << ", " << b << std::endl;
				if( b==true ) {
					std::cout << " checked "<< i << ", size " << gate->size() << std::endl;
					check_and_cast<IPassiveQueue *>(gate->getOwnerModule())->request(gate->getIndex());
				}*/
			}
        }
    }
    else
    {
        if (jobServiced) {
        	error("job arrived while already servicing one");
            std::cout << "job arrived while already servicing one" << std::endl;
        }

        jobServiced = check_and_cast<Job *>(msg);
        simtime_t serviceTime = par("serviceTime");
        scheduleAt(simTime()+serviceTime, endServiceMsg);
        emit(busySignal, 1);

        if (ev.isGUI()) getDisplayString().setTagArg("i",1,"cyan");
    }

#endif
#endif
}

IPassiveQueue *Server::getQueue(int index) {
	std::string queue = "passiveQueue";
	char buffer[3];

	sprintf(buffer,"%d",index);
	buffer[2]='\0';
	queue += buffer;
	cModule *module = getParentModule()->getSubmodule(queue.c_str());
	IPassiveQueue *pqueue = dynamic_cast<IPassiveQueue *>(module);

	return pqueue;
}

void Server::finish()
{
}

bool Server::isIdle()
{
    return jobServiced == NULL;
}

}; //namespace

