/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):	Balazs Vagvolgyi, Simon DiMaio, Anton Deguet
  Created on:	2008-05-23

  (C) Copyright 2008 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#include <cisstOSAbstraction/osaThreadedLogFile.h>
#include <cisstOSAbstraction/osaSleep.h>
#include <cisstMultiTask/mtsTaskManager.h>
#include <cisstDevices/devSensableHD.h>

#include "SimpleBehavior.h"

#include <vtkActor.h>
#include <vtkAssembly.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>

class SimpleBehaviorVisibleObject: public ui3VisibleObject
{
    CMN_DECLARE_SERVICES(CMN_NO_DYNAMIC_CREATION, 5);
public:
    inline SimpleBehaviorVisibleObject(ui3Manager * manager, vctDouble3 position):
        ui3VisibleObject(manager),
        Source(0),
        Mapper(0),
        Actor(0),
        Position(position),
        Red(true)
    {}

    inline ~SimpleBehaviorVisibleObject()
    {
        if (this->Actor) {
            this->Actor->Delete();
        }
        if (this->Mapper) {
            this->Mapper->Delete();
        }
        if (this->Source) {
            this->Source->Delete();
        }
    }

    inline bool CreateVTKObjects(void) {
        this->Source = vtkSphereSource::New();
        CMN_ASSERT(this->Source);
        this->Source->SetRadius(10.0);

        this->Mapper = vtkPolyDataMapper::New();
        CMN_ASSERT(this->Mapper);
        this->Mapper->SetInputConnection(this->Source->GetOutputPort());

        this->Actor = vtkActor::New();
        CMN_ASSERT(this->Actor);
        this->Actor->SetMapper(this->Mapper);
        this->Actor->GetProperty()->SetColor(1.0, 0.0, 0.0);

        this->Assembly->AddPart(this->Actor);
        this->SetPosition(this->Position);
        this->Hide();
        return true;
    }

    void ToggleColor(void) {
        if (this->Red) {
            this->Actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
            this->Red = false;
        } else {
            this->Actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
            this->Red = true;
        }
    }

protected:
    vtkSphereSource * Source;
    vtkPolyDataMapper * Mapper;
    vtkActor * Actor;
    vctDouble3 Position; // initial position
    bool Red;
};

CMN_DECLARE_SERVICES_INSTANTIATION(SimpleBehaviorVisibleObject);
CMN_IMPLEMENT_SERVICES(SimpleBehaviorVisibleObject);


SimpleBehavior::SimpleBehavior(const std::string & name, ui3Manager * manager):
    ui3BehaviorBase(std::string("SimpleBehavior::") + name, 0),
    Following(false),
    VisibleObject(0)
{
    this->Position.X() = 0.0;
    this->Position.Y() = 0.0;
    this->Position.Z() = -100.0;
    this->VisibleObject = new SimpleBehaviorVisibleObject(manager, this->Position);
    CMN_ASSERT(this->VisibleObject);
    this->VisibleObject->Show();
}


SimpleBehavior::~SimpleBehavior()
{
    if (this->VisibleObject) {
        delete this->VisibleObject;
    }
}

void SimpleBehavior::ConfigureMenuBar()
{
    this->MenuBar->AddClickButton("ToggleColor",
                                  1,
                                  "redo.png",
                                  &SimpleBehaviorVisibleObject::ToggleColor,
                                  dynamic_cast<SimpleBehaviorVisibleObject *>(this->VisibleObject));
}


bool SimpleBehavior::RunForeground()
{
    if (this->Manager->MastersAsMice() != this->PreviousMaM) {
        this->PreviousMaM = this->Manager->MastersAsMice();
        this->VisibleObject->Show();
    }

    // detect transition, should that be handled as an event?
    // State is used by multiple threads ...
    if (this->State != this->PreviousState) {
        this->PreviousState = this->State;
        this->VisibleObject->Show();
    }
    // running in foreground GUI mode
    prmPositionCartesianGet position;

    this->GetPrimaryMasterPosition(position);

    if (this->Following) {
        vctDouble3 deltaCursor;
        deltaCursor.DifferenceOf(position.Position().Translation(),
                                 this->PreviousCursorPosition);
        this->Position.Add(deltaCursor);
        this->VisibleObject->SetPosition(this->Position);
    }
    this->PreviousCursorPosition.Assign(position.Position().Translation());
    return true;
}

bool SimpleBehavior::RunBackground()
{
    // detect transition
    if (this->State != this->PreviousState) {
        this->PreviousState = this->State;
        this->VisibleObject->Hide();
    }
    return true;
}

bool SimpleBehavior::RunNoInput()
{
    if (this->Manager->MastersAsMice() != this->PreviousMaM) {
        this->PreviousMaM = this->Manager->MastersAsMice();
        this->VisibleObject->Hide();
    }
    return true;
}


void SimpleBehavior::OnQuit()
{
    this->VisibleObject->Hide();
}


void SimpleBehavior::OnStart()
{
    this->Position.X() = 0.0;
    this->Position.Y() = 0.0;
    this->Position.Z() = -100.0;
    this->VisibleObject->Show();
}


void SimpleBehavior::PrimaryMasterButtonCallback(const prmEventButton & event)
{
    if (event.Type() == prmEventButton::PRESSED) {
        this->Following = true;
    } else if (event.Type() == prmEventButton::RELEASED) {
        this->Following = false;
    }
}