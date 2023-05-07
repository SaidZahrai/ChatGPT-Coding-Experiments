#include "states.h"
#include <iostream> // include necessary header files

// Constructor for StateA, calling the constructor of StateBase with the name "A" and the pointer in the argument list
StateA::StateA(MachineBase* machine_ptr)
    : StateBase("A", machine_ptr) {}

// 'action' function for StateA
int StateA::action() {
    std::cout << "State A" << std::endl; // print the name of the state
    get_machine()->set_current_state("B"); // set the current state of the machine to StateB
    return 0; // return 0 on success
}

// Constructor for StateB, calling the constructor of StateBase with the name "B" and the pointer in the argument list
StateB::StateB(MachineBase* machine_ptr)
    : StateBase("B", machine_ptr) {}

// 'action' function for StateB
int StateB::action() {
    std::cout << "State B" << std::endl; // print the name of the state
    get_machine()->set_current_state("C"); // set the current state of the machine to StateC
    return 0; // return 0 on success
}

// Constructor for StateC, calling the constructor of StateBase with the name "C" and the pointer in the argument list
StateC::StateC(MachineBase* machine_ptr)
    : StateBase("C", machine_ptr) {}

// 'action' function for StateC
int StateC::action() {
    std::cout << "State C" << std::endl; // print the name of the state
    get_machine()->set_current_state("A"); // set the current state of the machine to StateA
    return 0; // return 0 on success
}
