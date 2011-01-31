/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Min Yang Jung
  Created on: 2010-08-29

  (C) Copyright 2010 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

/*!
  \brief Declaration of Manager Component Client
  \ingroup cisstMultiTask

  This class defines the manager component client which is managed by all local 
  component managers (LCMs).  An instance of this class is automatically created 
  and gets connected to the manager component server which runs on LCM that runs
  with the global component manager (GCM).
  
  This component has two sets of interfaces, one for communication with the 
  manager component server and the other one for command exchange between other
  manager component clients.
  
  \note Related classes: mtsManagerComponentBase, mtsManagerComponentServer
*/

#ifndef _mtsManagerComponentClient_h
#define _mtsManagerComponentClient_h

#include <cisstMultiTask/mtsManagerComponentBase.h>

class mtsManagerComponentClient : public mtsManagerComponentBase
{
    CMN_DECLARE_SERVICES(CMN_NO_DYNAMIC_CREATION, CMN_LOG_ALLOW_DEFAULT);

protected:
    /*! Get a list of all processes running in the system */
    mtsFunctionRead GetNamesOfProcesses;

    /*! Functions for InterfaceComponent's required interface.  Since one 
        manager component client needs to be able to handle multiple user 
        components, we keep a list of function objects using named map with 
        (key = component name, value = function object set instance) */
    typedef struct {
        mtsFunctionWrite ComponentStop;
        mtsFunctionWrite ComponentResume;
    } InterfaceComponentFunctionType;

    typedef cmnNamedMap<InterfaceComponentFunctionType> InterfaceComponentFunctionMapType;
    InterfaceComponentFunctionMapType InterfaceComponentFunctionMap;

    /*! Functions for InterfaceLCM's required interface */
    typedef struct {
        // Dynamic component management
        mtsFunctionWrite ComponentCreate;
        mtsFunctionWrite ComponentConnect;
        mtsFunctionWrite ComponentStart;
        mtsFunctionWrite ComponentStop;
        mtsFunctionWrite ComponentResume;
        // Getters
        mtsFunctionRead          GetNamesOfProcesses;
        mtsFunctionQualifiedRead GetNamesOfComponents; // in: process name, out: components' names
        mtsFunctionQualifiedRead GetNamesOfInterfaces; // in: process name, out: interfaces' names
        mtsFunctionRead          GetListOfConnections;
    } InterfaceLCMFunctionType;

    // Event handlers for InterfaceLCM's required interface
    void HandleAddComponentEvent(const mtsDescriptionComponent &component);
    void HandleAddConnectionEvent(const mtsDescriptionConnection &component);

    // Event handlers for InterfaceComponent's required interface
    void HandleChangeState(const mtsComponentStateChange & componentStateChange);

    InterfaceLCMFunctionType InterfaceLCMFunction;

    /*! Create new component and add it to LCM */
    bool CreateAndAddNewComponent(const std::string & className, const std::string & componentName);

public:
    mtsManagerComponentClient(const std::string & componentName);
    ~mtsManagerComponentClient();

    void Startup(void);
    void Run(void);
    void Cleanup(void);

    bool AddInterfaceLCM(void);
    bool AddInterfaceComponent(void);

    /*! Create a new set of function objects, add a new instance of 
        InterfaceComponent's required interface to this component, and connect 
        it to InterfaceInternal's provided interface */
    bool AddNewClientComponent(const std::string & clientComponentName);

    /*! Commands for InterfaceLCM's provided interface */
    void InterfaceLCMCommands_ComponentCreate(const mtsDescriptionComponent & arg);
    void InterfaceLCMCommands_ComponentConnect(const mtsDescriptionConnection & arg);
    void InterfaceLCMCommands_ComponentStart(const mtsComponentStatusControl & arg);
    void InterfaceLCMCommands_ComponentStop(const mtsComponentStatusControl & arg);
    void InterfaceLCMCommands_ComponentResume(const mtsComponentStatusControl & arg);

    /*! Commands for InterfaceComponent's provided interface */
    void InterfaceComponentCommands_ComponentCreate(const mtsDescriptionComponent & arg);
    void InterfaceComponentCommands_ComponentConnect(const mtsDescriptionConnection & arg);
    void InterfaceComponentCommands_ComponentStart(const mtsComponentStatusControl & arg);
    void InterfaceComponentCommands_ComponentStop(const mtsComponentStatusControl & arg);
    void InterfaceComponentCommands_ComponentResume(const mtsComponentStatusControl & arg);

    void InterfaceComponentCommands_GetNamesOfProcesses(mtsStdStringVec & names) const;
    void InterfaceComponentCommands_GetNamesOfComponents(const mtsStdString & processName, mtsStdStringVec & names) const;
    void InterfaceComponentCommands_GetNamesOfInterfaces(const mtsDescriptionComponent & component, mtsDescriptionInterface & interfaces) const;
    void InterfaceComponentCommands_GetListOfConnections(std::vector <mtsDescriptionConnection> & listOfConnections) const;

    /*! Event generators for InterfaceLCM's provided interface */
    mtsFunctionWrite InterfaceComponentEvents_AddComponent;
    mtsFunctionWrite InterfaceComponentEvents_AddConnection;

    /*! Returns name of manager component client */
    static std::string GetNameOfManagerComponentClient(const std::string & processName);
    
    /*! Returns name of InterfaceComponent's required interface */
    static std::string GetNameOfInterfaceComponentRequired(const std::string & userComponentName);
};

CMN_DECLARE_SERVICES_INSTANTIATION(mtsManagerComponentClient);

#endif // _mtsManagerComponentClient_h