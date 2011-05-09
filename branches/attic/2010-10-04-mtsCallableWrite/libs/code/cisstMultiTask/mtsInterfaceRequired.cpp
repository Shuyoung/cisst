/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Peter Kazanzides, Anton Deguet
  Created on: 2008-11-13

  (C) Copyright 2008-2010 Johns Hopkins University (JHU), All Rights Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#include <cisstMultiTask/mtsInterfaceRequired.h>

#include <cisstMultiTask/mtsFunctionVoid.h>
#include <cisstMultiTask/mtsFunctionVoidReturn.h>
#include <cisstMultiTask/mtsFunctionWrite.h>
#include <cisstMultiTask/mtsFunctionRead.h>
#include <cisstMultiTask/mtsFunctionQualifiedRead.h>
#include <cisstMultiTask/mtsEventReceiver.h>

#include <cisstCommon/cmnSerializer.h>
#include <cisstMultiTask/mtsInterfaceProvided.h>

mtsInterfaceRequired::mtsInterfaceRequired(const std::string & interfaceName,
                                           mtsMailBox * mailBox,
                                           mtsRequiredType required):
    mtsInterfaceRequiredOrInput(interfaceName, required),
    MailBox(mailBox),
    FunctionsVoid("FunctionsVoid"),
    FunctionsVoidReturn("FunctionsVoidReturn"),
    FunctionsRead("FunctionsRead"),
    FunctionsWrite("FunctionsWrite"),
    FunctionsQualifiedRead("FunctionsQualifiedRead"),
    EventReceiversVoid("EventReceiversVoid"),
    EventReceiversWrite("EventReceiversWrite"),
    EventHandlersVoid("EventHandlersVoid"),
    EventHandlersWrite("EventHandlersWrite")
{
    FunctionsVoid.SetOwner(*this);
    FunctionsVoidReturn.SetOwner(*this);
    FunctionsRead.SetOwner(*this);
    FunctionsWrite.SetOwner(*this);
    FunctionsQualifiedRead.SetOwner(*this);
    EventReceiversVoid.SetOwner(*this);
    EventReceiversWrite.SetOwner(*this);
    EventHandlersVoid.SetOwner(*this);
    EventHandlersWrite.SetOwner(*this);
}


mtsInterfaceRequired::~mtsInterfaceRequired()
{
    FunctionsVoid.DeleteAll();
    FunctionsVoidReturn.DeleteAll();
    FunctionsRead.DeleteAll();
    FunctionsWrite.DeleteAll();
    FunctionsQualifiedRead.DeleteAll();
}


bool mtsInterfaceRequired::UseQueueBasedOnInterfacePolicy(mtsEventQueueingPolicy queueingPolicy,
                                                          const std::string & methodName,
                                                          const std::string & eventName)
{
    bool interfaceHasMailbox = (this->MailBox != 0);
    if (interfaceHasMailbox) {
        if (queueingPolicy == MTS_EVENT_NOT_QUEUED) {
            CMN_LOG_CLASS_INIT_DEBUG << methodName << ": event handler for \"" << eventName
                                     << "\" will not be queued while the corresponding required interface \""
                                     << this->GetName() << "\" uses queued event handlers by default." << std::endl;
            return false;
        } else {
            return true;
        }
    } else {
        if (queueingPolicy == MTS_EVENT_QUEUED) {
            CMN_LOG_CLASS_INIT_ERROR  << methodName << ": event handler for \"" << eventName
                                      << "\" has been added as queued while the corresponding required interface \""
                                      << this->GetName() << "\" has been created without a mailbox." << std::endl;
            return true;
        } else {
            return false;
        }
    }
}


bool mtsInterfaceRequired::AddEventHandlerToReceiver(const std::string & eventName, mtsCommandVoid * handler) const
{
    bool result = false;
    ReceiverVoidInfo * receiverInfo = EventReceiversVoid.GetItem(eventName, CMN_LOG_LOD_RUN_VERBOSE);
    if (receiverInfo) {
        receiverInfo->Pointer->SetHandlerCommand(handler);
        result = true;
    }
    return result;
}


bool mtsInterfaceRequired::AddEventHandlerToReceiver(const std::string & eventName, mtsCommandWrite * handler) const
{
    bool result = false;
    ReceiverWriteInfo * receiverInfo = EventReceiversWrite.GetItem(eventName, CMN_LOG_LOD_INIT_WARNING);
    if (receiverInfo) {
        receiverInfo->Pointer->SetHandlerCommand(handler);
        result = true;
    }
    return result;
}


std::vector<std::string> mtsInterfaceRequired::GetNamesOfFunctions(void) const {
    std::vector<std::string> commands = GetNamesOfFunctionsVoid();
    std::vector<std::string> tmp = GetNamesOfFunctionsVoidReturn();
    commands.insert(commands.begin(), tmp.begin(), tmp.end());
    tmp.clear();
    tmp = GetNamesOfFunctionsRead();
    commands.insert(commands.begin(), tmp.begin(), tmp.end());
    tmp.clear();
    tmp = GetNamesOfFunctionsWrite();
    commands.insert(commands.begin(), tmp.begin(), tmp.end());
    tmp.clear();
    tmp = GetNamesOfFunctionsQualifiedRead();
    commands.insert(commands.begin(), tmp.begin(), tmp.end());
    return commands;
}


std::vector<std::string> mtsInterfaceRequired::GetNamesOfFunctionsVoid(void) const {
    return FunctionsVoid.GetNames();
}


std::vector<std::string> mtsInterfaceRequired::GetNamesOfFunctionsVoidReturn(void) const {
    return FunctionsVoidReturn.GetNames();
}


std::vector<std::string> mtsInterfaceRequired::GetNamesOfFunctionsRead(void) const {
    return FunctionsRead.GetNames();
}


std::vector<std::string> mtsInterfaceRequired::GetNamesOfFunctionsWrite(void) const {
    return FunctionsWrite.GetNames();
}


std::vector<std::string> mtsInterfaceRequired::GetNamesOfFunctionsQualifiedRead(void) const {
    return FunctionsQualifiedRead.GetNames();
}


std::vector<std::string> mtsInterfaceRequired::GetNamesOfEventHandlersVoid(void) const {
    return EventHandlersVoid.GetNames();
}


std::vector<std::string> mtsInterfaceRequired::GetNamesOfEventHandlersWrite(void) const {
    return EventHandlersWrite.GetNames();
}


mtsCommandVoid * mtsInterfaceRequired::GetEventHandlerVoid(const std::string & eventName) const {
    return EventHandlersVoid.GetItem(eventName);
}


mtsCommandWrite * mtsInterfaceRequired::GetEventHandlerWrite(const std::string & eventName) const {
    return EventHandlersWrite.GetItem(eventName);
}


bool mtsInterfaceRequired::ConnectTo(mtsInterfaceProvidedOrOutput * interfaceProvidedOrOutput) {
    // make sure we are connecting to a provided interface
    mtsInterfaceProvided * interfaceProvided = dynamic_cast<mtsInterfaceProvided *>(interfaceProvidedOrOutput);
    if (!interfaceProvided) {
        CMN_LOG_CLASS_INIT_ERROR << "ConnectTo: can not connect input interface \""
                                 << interfaceProvidedOrOutput->GetName() << "\" to required interface \""
                                 << this->GetName() << "\", can only connect required with provided or input with output"
                                 << std::endl;
        return false;
    }
    // get the end user interface
    mtsInterfaceProvided * endUserInterface = interfaceProvided->GetEndUserInterface(this->GetName());
    if (!endUserInterface) {
        CMN_LOG_CLASS_INIT_ERROR << "ConnectTo: failed to get an end user provided interface for \""
                                 << interfaceProvided->GetName() << "\" while connecting the required interface \""
                                 << this->GetName() << "\"" << std::endl;
        return false;
    }
    // provide a different log message based on result
    if (endUserInterface == interfaceProvided) {
        CMN_LOG_CLASS_INIT_VERBOSE << "ConnectTo: connecting required interface \""
                                   << this->GetName() << "\" to existing provided interface \""
                                   << endUserInterface->GetName() << "\"" << std::endl;
    } else {
        CMN_LOG_CLASS_INIT_VERBOSE << "ConnectTo: connecting required interface \""
                                   << this->GetName() << "\" to newly created provided interface \""
                                   << endUserInterface->GetName() << "\"" << std::endl;
    }
    this->InterfaceProvidedOrOutput = endUserInterface;
    // and now connect all commands and events
    return this->BindCommandsAndEvents();
}


bool mtsInterfaceRequired::Disconnect(void)
{
    // first, do the command pointers.  In the future, it may be
    // better to set the pointers to NOPVoid, NOPRead, etc., which can
    // be static members of the corresponding command classes.
    FunctionInfoMapType::iterator iter;
    for (iter = FunctionsVoid.begin(); iter != FunctionsVoid.end(); iter++)
        iter->second->Detach();
    for (iter = FunctionsVoidReturn.begin(); iter != FunctionsVoidReturn.end(); iter++)
        iter->second->Detach();
    for (iter = FunctionsRead.begin(); iter != FunctionsRead.end(); iter++)
        iter->second->Detach();
    for (iter = FunctionsWrite.begin(); iter != FunctionsWrite.end(); iter++)
        iter->second->Detach();
    for (iter = FunctionsQualifiedRead.begin(); iter != FunctionsQualifiedRead.end(); iter++)
        iter->second->Detach();
#if 0
    // Now, do the event handlers.  Still need to implement RemoveObserver
    EventHandlerVoidMapType::iterator iterEventVoid;
    for (iterEventVoid = EventHandlersVoid.begin(); iterEventVoid != EventHandlersVoid.end(); iterEventVoid++)
        InterfaceProvided->RemoveObserver(iterEventVoid->first, iterEventVoid->second);
    EventHandlerWriteMapType::iterator iterEventWrite;
    for (iterEventWrite = EventHandlersWrite.begin(); iterEventWrite != EventHandlersWrite.end(); iterEventWrite++)
        InterfaceProvided->RemoveObserver(iterEventWrite->first, iterEventWrite->second);
#endif
    return true;
}


bool mtsInterfaceRequired::BindCommandsAndEvents(void)
{
    bool success = true;
    bool result;
    mtsInterfaceProvided * interfaceProvided =
        dynamic_cast<mtsInterfaceProvided *>(this->InterfaceProvidedOrOutput);
    if (!interfaceProvided) {
        CMN_LOG_CLASS_INIT_ERROR << "BindCommandsAndEvents: required interface \""
                                 << this->GetName() << "\" is not connected to a valid provided interface" << std::endl;
        return false;
    }
    // First, do the command pointers
    FunctionInfoMapType::iterator iter;
    mtsFunctionVoid * functionVoid;
    for (iter = FunctionsVoid.begin();
         iter != FunctionsVoid.end();
         iter++) {
        if (!iter->second->Pointer) {
            CMN_LOG_CLASS_INIT_ERROR << "BindCommandsAndEvents: found null function pointer for void command \""
                                     << iter->first << "\" in interface \"" << this->GetName() << "\"" << std::endl;
            result = false;
        } else {
            functionVoid = dynamic_cast<mtsFunctionVoid *>(iter->second->Pointer);
            if (!functionVoid) {
                CMN_LOG_CLASS_INIT_ERROR << "BindCommandsAndEvents: incorrect function pointer for void command \""
                                         << iter->first << "\" in interface \"" << this->GetName() << "\" (got \""
                                         << typeid(iter->second->Pointer).name() << "\")" << std::endl;
                result = false;
            } else {
                result = functionVoid->Bind(interfaceProvided->GetCommandVoid(iter->first));
                if (!result) {
                    CMN_LOG_CLASS_INIT_WARNING << "BindCommandsAndEvents: failed for void command \""
                                               << iter->first << "\" (connecting \""
                                               << this->GetName() << "\" to \""
                                               << interfaceProvided->GetName() << "\")"<< std::endl;
                } else {
                    CMN_LOG_CLASS_INIT_DEBUG << "BindCommandsAndEvents: succeeded for void command \""
                                             << iter->first << "\" (connecting \""
                                             << this->GetName() << "\" to \""
                                             << interfaceProvided->GetName() << "\")"<< std::endl;
                }
            }
        }
        if (iter->second->Required == MTS_OPTIONAL) {
            result = true;
        }
        success &= result;
    }

    mtsFunctionVoidReturn * functionVoidReturn;
    for (iter = FunctionsVoidReturn.begin();
         iter != FunctionsVoidReturn.end();
         iter++) {
        if (!iter->second->Pointer) {
            CMN_LOG_CLASS_INIT_ERROR << "BindCommandsAndEvents: found null function pointer for void with return command \""
                                     << iter->first << "\" in interface \"" << this->GetName() << "\"" << std::endl;
            result = false;
        } else {
            functionVoidReturn = dynamic_cast<mtsFunctionVoidReturn *>(iter->second->Pointer);
            if (!functionVoidReturn) {
                CMN_LOG_CLASS_INIT_ERROR << "BindCommandsAndEvents: incorrect function pointer for void with return command \""
                                         << iter->first << "\" in interface \"" << this->GetName() << "\" (got \""
                                         << typeid(iter->second->Pointer).name() << "\")" << std::endl;
                result = false;
            } else {
                result = functionVoidReturn->Bind(interfaceProvided->GetCommandVoidReturn(iter->first));
                if (!result) {
                    CMN_LOG_CLASS_INIT_WARNING << "BindCommandsAndEvents: failed for void with return command \""
                                               << iter->first << "\" (connecting \""
                                               << this->GetName() << "\" to \""
                                               << interfaceProvided->GetName() << "\")"<< std::endl;
                } else {
                    CMN_LOG_CLASS_INIT_DEBUG << "BindCommandsAndEvents: succeeded for void with return command \""
                                             << iter->first << "\" (connecting \""
                                             << this->GetName() << "\" to \""
                                             << interfaceProvided->GetName() << "\")"<< std::endl;
                }
            }
        }
        if (iter->second->Required == MTS_OPTIONAL) {
            result = true;
        }
        success &= result;
    }

    mtsFunctionRead * functionRead;
    for (iter = FunctionsRead.begin();
         iter != FunctionsRead.end();
         iter++) {
        if (!iter->second->Pointer) {
            CMN_LOG_CLASS_INIT_ERROR << "BindCommandsAndEvents: found null function pointer for read command \""
                                     << iter->first << "\" in interface \"" << this->GetName() << "\"" << std::endl;
            result = false;
        } else {
            functionRead = dynamic_cast<mtsFunctionRead *>(iter->second->Pointer);
            if (!functionRead) {
                CMN_LOG_CLASS_INIT_ERROR << "BindCommandsAndEvents: incorrect function pointer for read command \""
                                         << iter->first << "\" in interface \"" << this->GetName() << "\" (got \""
                                         << typeid(iter->second->Pointer).name() << "\")" << std::endl;
                result = false;
            } else {
                result = functionRead->Bind(interfaceProvided->GetCommandRead(iter->first));
                if (!result) {
                    CMN_LOG_CLASS_INIT_WARNING << "BindCommandsAndEvents: failed for read command \""
                                               << iter->first << "\" (connecting \""
                                               << this->GetName() << "\" to \""
                                               << interfaceProvided->GetName() << "\")"<< std::endl;
                } else {
                    CMN_LOG_CLASS_INIT_DEBUG << "BindCommandsAndEvents: succeeded for read command \""
                                             << iter->first  << "\" (connecting \""
                                             << this->GetName() << "\" to \""
                                             << interfaceProvided->GetName() << "\")"<< std::endl;
                }
            }
        }
        if (iter->second->Required == MTS_OPTIONAL) {
            result = true;
        }
        success &= result;
    }

    mtsFunctionWrite * functionWrite;
    for (iter = FunctionsWrite.begin();
         iter != FunctionsWrite.end();
         iter++) {
        if (!iter->second->Pointer) {
            CMN_LOG_CLASS_INIT_ERROR << "BindCommandsAndEvents: found null function pointer for write command \""
                                     << iter->first << "\" in interface \"" << this->GetName() << "\"" << std::endl;
            result = false;
        } else {
            functionWrite = dynamic_cast<mtsFunctionWrite *>(iter->second->Pointer);
            if (!functionWrite) {
                CMN_LOG_CLASS_INIT_ERROR << "BindCommandsAndEvents: incorrect function pointer for write command \""
                                         << iter->first << "\" in interface \"" << this->GetName() << "\" (got \""
                                         << typeid(iter->second->Pointer).name() << "\")" << std::endl;
                result = false;
            } else {
                result = functionWrite->Bind(interfaceProvided->GetCommandWrite(iter->first));
                if (!result) {
                    CMN_LOG_CLASS_INIT_WARNING << "BindCommandsAndEvents: failed for write command \""
                                               << iter->first << "\" (connecting \""
                                               << this->GetName() << "\" to \""
                                               << interfaceProvided->GetName() << "\")"<< std::endl;
                } else {
                    CMN_LOG_CLASS_INIT_DEBUG << "BindCommandsAndEvents: succeeded for write command \""
                                             << iter->first << "\" (connecting \""
                                             << this->GetName() << "\" to \""
                                             << interfaceProvided->GetName() << "\")"<< std::endl;
                }
            }
        }
        if (iter->second->Required == MTS_OPTIONAL) {
            result = true;
        }
        success &= result;
    }

    mtsFunctionQualifiedRead * functionQualifiedRead;
    for (iter = FunctionsQualifiedRead.begin();
         iter != FunctionsQualifiedRead.end();
         iter++) {
        if (!iter->second->Pointer) {
            CMN_LOG_CLASS_INIT_ERROR << "BindCommandsAndEvents: found null function pointer for qualified read command \""
                                     << iter->first << "\" in interface \"" << this->GetName() << "\"" << std::endl;
            result = false;
        } else {
            functionQualifiedRead = dynamic_cast<mtsFunctionQualifiedRead *>(iter->second->Pointer);
            if (!functionQualifiedRead) {
                CMN_LOG_CLASS_INIT_ERROR << "BindCommandsAndEvents: incorrect function pointer for qualified read command \""
                                         << iter->first << "\" in interface \"" << this->GetName() << "\" (got \""
                                         << typeid(iter->second->Pointer).name() << "\")" << std::endl;
                result = false;
            } else {
                result = functionQualifiedRead->Bind(interfaceProvided->GetCommandQualifiedRead(iter->first));
                if (!result) {
                    CMN_LOG_CLASS_INIT_WARNING << "BindCommandsAndEvents: failed for qualified read command \""
                                               << iter->first << "\" (connecting \""
                                               << this->GetName() << "\" to \""
                                               << interfaceProvided->GetName() << "\")"<< std::endl;
                } else {
                    CMN_LOG_CLASS_INIT_DEBUG << "BindCommandsAndEvents: succeeded for qualified read command \""
                                             << iter->first << "\" (connecting \""
                                             << this->GetName() << "\" to \""
                                             << interfaceProvided->GetName() << "\")"<< std::endl;
                }
            }
        }
        if (iter->second->Required == MTS_OPTIONAL) {
            result = true;
        }
        success &= result;
    }

    if (!success) {
        CMN_LOG_CLASS_INIT_ERROR << "BindCommandsAndEvents: required commands missing (connecting \""
                                 << this->GetName() << "\" to \""
                                 << interfaceProvided->GetName() << "\")"<< std::endl;
    }

    // Now, the event receivers
    EventReceiverVoidMapType::iterator iterReceiverVoid;
    for (iterReceiverVoid = EventReceiversVoid.begin();
         iterReceiverVoid != EventReceiversVoid.end();
         iterReceiverVoid++) {
        result = interfaceProvided->AddObserver(iterReceiverVoid->first, iterReceiverVoid->second->Pointer->GetCommand());
        if (!result) {
            CMN_LOG_CLASS_INIT_WARNING << "BindCommandsAndEvents: failed to add observer for void event receiver \""
                                       << iterReceiverVoid->first << "\" (connecting \""
                                       << this->GetName() << "\" to \""
                                       << interfaceProvided->GetName() << "\")"<< std::endl;
        } else {
            CMN_LOG_CLASS_INIT_DEBUG << "BindCommandsAndEvents: succeeded to add observer for void event receiver \""
                                     << iterReceiverVoid->first << "\" (connecting \""
                                     << this->GetName() << "\" to \""
                                     << interfaceProvided->GetName() << "\")"<< std::endl;
        }
        if (iterReceiverVoid->second->Required == MTS_OPTIONAL) {
            result = true;
        }
        success &= result;
    }
    EventReceiverWriteMapType::iterator iterReceiverWrite;
    for (iterReceiverWrite = EventReceiversWrite.begin();
         iterReceiverWrite != EventReceiversWrite.end();
         iterReceiverWrite++) {
        result = interfaceProvided->AddObserver(iterReceiverWrite->first, iterReceiverWrite->second->Pointer->GetCommand());
        if (!result) {
            CMN_LOG_CLASS_INIT_WARNING << "BindCommandsAndEvents: failed to add observer for write event receiver \""
                                       << iterReceiverWrite->first << "\" (connecting \""
                                       << this->GetName() << "\" to \""
                                       << interfaceProvided->GetName() << "\")"<< std::endl;
        } else {
            CMN_LOG_CLASS_INIT_DEBUG << "BindCommandsAndEvents: succeeded to add observer for write event receiver \""
                                     << iterReceiverWrite->first << "\" (connecting \""
                                     << this->GetName() << "\" to \""
                                     << interfaceProvided->GetName() << "\")"<< std::endl;
        }
        if (iterReceiverWrite->second->Required == MTS_OPTIONAL) {
            result = true;
        }
        success &= result;
    }


    // Now, do the event handlers
    EventHandlerVoidMapType::iterator iterEventVoid;
    for (iterEventVoid = EventHandlersVoid.begin();
         iterEventVoid != EventHandlersVoid.end();
         iterEventVoid++) {
        // First, attempt to add it to the event receiver (if one is present)
        result = AddEventHandlerToReceiver(iterEventVoid->first, iterEventVoid->second);
        // If there is no event receiver, add it directly to the provided interface
        if (!result)
            result = interfaceProvided->AddObserver(iterEventVoid->first, iterEventVoid->second);
        if (!result) {
            CMN_LOG_CLASS_INIT_WARNING << "BindCommandsAndEvents: failed to add observer for void event \""
                                       << iterEventVoid->first << "\" (connecting \""
                                       << this->GetName() << "\" to \""
                                       << interfaceProvided->GetName() << "\")"<< std::endl;
        } else {
            CMN_LOG_CLASS_INIT_DEBUG << "BindCommandsAndEvents: succeeded to add observer for void event \""
                                     << iterEventVoid->first << "\" (connecting \""
                                     << this->GetName() << "\" to \""
                                     << interfaceProvided->GetName() << "\")"<< std::endl;
        }
    }

    EventHandlerWriteMapType::iterator iterEventWrite;
    for (iterEventWrite = EventHandlersWrite.begin();
         iterEventWrite != EventHandlersWrite.end();
         iterEventWrite++) {
        // First, attempt to add it to the event receiver (if one is present)
        result = AddEventHandlerToReceiver(iterEventWrite->first, iterEventWrite->second);
        // If there is no event receiver, add it directly to the provided interface
        if (!result)
            result = interfaceProvided->AddObserver(iterEventWrite->first, iterEventWrite->second);
        if (!result) {
            CMN_LOG_CLASS_INIT_WARNING << "BindCommandsAndEvents: failed to add observer for write event \""
                                       << iterEventWrite->first << "\" (connecting \""
                                       << this->GetName() << "\" to \""
                                       << interfaceProvided->GetName() << "\")"<< std::endl;
        } else {
            CMN_LOG_CLASS_INIT_DEBUG << "BindCommandsAndEvents: succeeded to add observer for write event \""
                                     << iterEventWrite->first << "\" (connecting \""
                                     << this->GetName() << "\" to \""
                                     << interfaceProvided->GetName() << "\")"<< std::endl;
        }
    }

    return success;
}


void mtsInterfaceRequired::DisableAllEvents(void) {
    EventHandlersVoid.ForEachVoid(&mtsCommandBase::Disable);
    EventHandlersWrite.ForEachVoid(&mtsCommandBase::Disable);
}


void mtsInterfaceRequired::EnableAllEvents(void) {
    EventHandlersVoid.ForEachVoid(&mtsCommandBase::Enable);
    EventHandlersWrite.ForEachVoid(&mtsCommandBase::Enable);
}


size_t mtsInterfaceRequired::ProcessMailBoxes(void)
{
    // MJUNG: Currently, one of constructor of mtsInterfaceRequired allows
    // a null pointer to be passed as the second argument.
    if (!MailBox) {
        return 0;
    }

    unsigned int numberOfCommands = 0;
    while (MailBox->ExecuteNext()) {
        numberOfCommands++;
    }
    return numberOfCommands;
}


void mtsInterfaceRequired::ToStream(std::ostream & outputStream) const
{
    outputStream << "Required Interface name: " << Name << std::endl;
    FunctionsVoid.ToStream(outputStream);
    FunctionsRead.ToStream(outputStream);
    FunctionsWrite.ToStream(outputStream);
    FunctionsQualifiedRead.ToStream(outputStream);
    EventHandlersVoid.ToStream(outputStream);
    EventHandlersWrite.ToStream(outputStream);
}


bool mtsInterfaceRequired::AddFunction(const std::string & functionName, mtsFunctionVoid & function,
                                       mtsRequiredType required)
{
    return FunctionsVoid.AddItem(functionName, new FunctionInfo(function, required));
}


bool mtsInterfaceRequired::AddFunction(const std::string & functionName, mtsFunctionVoidReturn & function,
                                       mtsRequiredType required)
{
    return FunctionsVoidReturn.AddItem(functionName, new FunctionInfo(function, required));
}


bool mtsInterfaceRequired::AddFunction(const std::string & functionName, mtsFunctionRead & function,
                                       mtsRequiredType required)
{
    return FunctionsRead.AddItem(functionName, new FunctionInfo(function, required));
}


bool mtsInterfaceRequired::AddFunction(const std::string & functionName, mtsFunctionWrite & function,
                                       mtsRequiredType required)
{
    return FunctionsWrite.AddItem(functionName, new FunctionInfo(function, required));
}


bool mtsInterfaceRequired::AddFunction(const std::string & functionName, mtsFunctionQualifiedRead & function,
                                       mtsRequiredType required)
{
    return FunctionsQualifiedRead.AddItem(functionName, new FunctionInfo(function, required));
}


bool mtsInterfaceRequired::AddEventReceiver(const std::string & eventName, mtsEventReceiverVoid & receiver, mtsRequiredType required)
{
    receiver.SetRequired(eventName, this);
    return EventReceiversVoid.AddItem(eventName, new ReceiverVoidInfo(receiver, required));
}


bool mtsInterfaceRequired::AddEventReceiver(const std::string & eventName, mtsEventReceiverWrite & receiver, mtsRequiredType required)
{
    receiver.SetRequired(eventName, this);
    return EventReceiversWrite.AddItem(eventName, new ReceiverWriteInfo(receiver, required));
}


mtsCommandVoid * mtsInterfaceRequired::AddEventHandlerVoid(mtsCallableVoidBase * callable,
                                                           const std::string & eventName,
                                                           mtsEventQueueingPolicy queueingPolicy)
{
    CMN_ASSERT(callable);
    bool queued = this->UseQueueBasedOnInterfacePolicy(queueingPolicy, "AddEventHandlerVoid", eventName);
    if (queued) {
        if (MailBox) {
            EventHandlersVoid.AddItem(eventName, new mtsCommandQueuedVoid(callable, eventName, MailBox, DEFAULT_EVENT_QUEUE_LEN));
        } else {
            CMN_LOG_CLASS_INIT_ERROR << "No mailbox for queued event handler void \"" << eventName << "\"" << std::endl;
        }
    } else {
        EventHandlersVoid.AddItem(eventName, new mtsCommandVoid(callable, eventName));
    }
    mtsCommandVoid * handler = EventHandlersVoid.GetItem(eventName);
    AddEventHandlerToReceiver(eventName, handler);  // does nothing if event receiver does not exist
    return handler;
}


mtsCommandWrite * mtsInterfaceRequired::AddEventHandlerWrite(mtsCallableWriteBase * callable,
                                                             const std::string & eventName,
                                                             mtsEventQueueingPolicy queueingPolicy)
{
    CMN_ASSERT(callable);
    bool queued = this->UseQueueBasedOnInterfacePolicy(queueingPolicy, "AddEventHandlerWrite", eventName);
    if (queued) {
        if (MailBox) {
            EventHandlersWrite.AddItem(eventName, new mtsCommandQueuedWrite(callable, eventName, 0, MailBox, DEFAULT_EVENT_QUEUE_LEN));
        } else {
            CMN_LOG_CLASS_INIT_ERROR << "No mailbox for queued event handler void \"" << eventName << "\"" << std::endl;
        }
    } else {
        EventHandlersWrite.AddItem(eventName, new mtsCommandWrite(callable, eventName, 0));
    }
    mtsCommandWrite * handler = EventHandlersWrite.GetItem(eventName);
    AddEventHandlerToReceiver(eventName, handler);  // does nothing if event receiver does not exist
    return handler;
}


bool mtsInterfaceRequired::RemoveEventHandlerVoid(const std::string &eventName)
{
    mtsCommandVoid * handler = 0;
    AddEventHandlerToReceiver(eventName, handler);
    return EventHandlersVoid.RemoveItem(eventName, CMN_LOG_LOD_RUN_WARNING);
}


bool mtsInterfaceRequired::RemoveEventHandlerWrite(const std::string &eventName)
{
    mtsCommandWrite * handler = 0;
    AddEventHandlerToReceiver(eventName, handler);
    return EventHandlersWrite.RemoveItem(eventName, CMN_LOG_LOD_RUN_WARNING);
}