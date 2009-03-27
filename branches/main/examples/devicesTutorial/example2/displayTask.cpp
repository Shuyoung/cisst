/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */
/* $Id: displayTask.cpp 163 2009-03-17 16:27:35Z adeguet1 $ */


#include "displayTask.h"
#include "displayUI.h"

CMN_IMPLEMENT_SERVICES(displayTask);

displayTask::displayTask(const std::string & taskName, double period):
    mtsTaskPeriodic(taskName, period, false, 500),
    ExitFlag(false)
{
    // to communicate with the interface of the resource
    mtsRequiredInterface * requiredInterface = AddRequiredInterface("Scale");
	if (requiredInterface) {
        // bound the mtsFunction to the command provided by the interface 
        requiredInterface->AddFunction("GetWeight", GetWeight, mtsRequired);
    }
}

void displayTask::Configure(const std::string & CMN_UNUSED(filename))
{
    // define some values, ideally these come from a configuration
    // file and then configure the user interface
}

void displayTask::Startup(void) 
{
    // make the UI visible
    UI.show(0, NULL);
}

void displayTask::Run(void)
{
    // process events
    this->ProcessQueuedEvents();

    // get the data from the scale
    GetWeight(this->Weight);
    UI.WeightValue->value(this->Weight.Data);

    // update the UI, process UI events 
    if (Fl::check() == 0) {
        ExitFlag = true;
    }
}


/*
  Author(s):  Anton Deguet
  Created on: 2009-03-27

  (C) Copyright 2009 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/
