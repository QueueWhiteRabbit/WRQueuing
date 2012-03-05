//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


#ifndef __WRS_H
#define __WRS_H

#include "AbstractWRS.h"
#include "Useful.h"
#include "Sink.h"

namespace wrs {

/**
 * Single-server queue with a given service time.
 */
class WRS : public AbstractWRS
{

  protected:
    virtual simtime_t startService(cMessage *msg);
    virtual void endService(cMessage *msg);

};

}; //namespace

#endif