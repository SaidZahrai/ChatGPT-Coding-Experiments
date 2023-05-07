#ifndef STATES_H_
#define STATES_H_

#include "state_base.h" // include necessary header file

class StateA : public StateBase {
public:
    StateA(MachineBase* machine_ptr) : StateBase("StateA", machine_ptr) {} // constructor with one argument, a pointer to a 'MachineBase' object
    virtual int action() override; // override the 'action' function
};

class StateB : public StateBase {
public:
    StateB(MachineBase* machine_ptr) : StateBase("StateB", machine_ptr) {} // constructor with one argument, a pointer to a 'MachineBase' object
    virtual int action() override; // override the 'action' function
};

class StateC : public StateBase {
public:
    StateC(MachineBase* machine_ptr) : StateBase("StateC", machine_ptr) {} // constructor with one argument, a pointer to a 'MachineBase' object
    virtual int action() override; // override the 'action' function
};

#endif // STATES_H_
