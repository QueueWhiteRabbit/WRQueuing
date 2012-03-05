//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

// SMa, 11.01.2012

#include "WRClassifier.h"
#include "Job.h"

namespace queueing {

Define_Module(WRClassifier);

void WRClassifier::initialize()
{
    dispatchField = par("dispatchField");
}

void WRClassifier::handleMessage(cMessage *msg)
{
    Job *job = check_and_cast<Job *>(msg);
    int priority = job->getPriority();
    int outGateIndex = -1;
    if (strcmp(dispatchField, "type") == 0)
        outGateIndex = job->getKind();
    else if (strcmp(dispatchField, "priority") == 0) {
        outGateIndex = job->getPriority();
        std::cout << "priority " << job->getPriority() << std::endl;
    }
    else
        error("invalid dispatchField parameter, must be \"type\" or \"priority\"");
    // TODO we could look for the value in the dynamically added parameters too

    //send(job, "out", outGateIndex);
    //std::cout << "sent to outGateIndex: " << outGateIndex << std::endl;

    std::string queue;
	//sprintf(queue, "wrQueue%d", priority);
	queue = "wrQueue";
	char buffer[3];

    sprintf(buffer,"%d\0",priority);
	std::cout << "buf " << buffer << std::endl;
	queue += buffer;
	std::cout << "Classifier in: send package with priority " << priority << " to " << queue << std::endl;
	cModule *targetModule = getParentModule()->getSubmodule(queue.c_str());
	sendDirect(msg, targetModule, "sendDirect");

    // SMa, 13.01.2012
    //if( priority<7 ) {
    /*	if (outGateIndex < 0 || outGateIndex >= gateSize("out")) {
			//send(job, "rest");
			//error("outGateIndex < 0 || outGateIndex >= gateSize(out): %d (%d)", outGateIndex, gateSize("out"));
			std::cout << "outGateIndex: " << outGateIndex << std::endl;
			//send(job, "out", 0);	// danger of overriding something?

			cModule *targetModule = getParentModule()->getSubmodule("switch");
			sendDirect(msg, targetModule, "sendDirect");
		} */
    /*} else {
    	// packets with highest priority send immediately
		cModule *targetModule = getParentModule()->getSubmodule("switch");
		sendDirect(msg, targetModule, "sendDirect");
    }*/

    /*
    if (outGateIndex < 0 || outGateIndex >= gateSize("out")) {
        //send(job, "rest");
        //error("outGateIndex < 0 || outGateIndex >= gateSize(out): %d (%d)", outGateIndex, gateSize("out"));
        std::cout << "outGateIndex: " << outGateIndex << std::endl;
    	//send(job, "out", 0);	// danger of overriding something?

        cModule *targetModule = getParentModule()->getSubmodule("wrQueue");
    	sendDirect(msg, targetModule, "sendDirect");
    }
    else if (outGateIndex>=7) {
    	// packets with highest priority send immediately
    	//send(job, "out", 0);	// danger of overriding something?
    	cModule *targetModule = getParentModule()->getSubmodule("switch");
		sendDirect(msg, targetModule, "sendDirect");
    }
    else {
    	// insert into queue that should be emptied if there is no high priority packet
    	// TODO
    	//send(job, "out", outGateIndex);
    	cModule *targetModule = getParentModule()->getSubmodule("wrQueue");
		sendDirect(msg, targetModule, "sendDirect");
    }*/

}

}; //namespace
