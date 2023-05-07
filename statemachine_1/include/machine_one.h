#ifndef MACHINE_ONE_H_
#define MACHINE_ONE_H_

#include "machine_base.h" // include necessary header file

template<typename... States>
class MachineOne : public MachineBase {
public:
    MachineOne() : MachineBase("MachineOne") { // constructor with no arguments, calling the constructor of the base class with the name "MachineOne"
        (add_state(new States(this)), ...); // loop over all classes in the template pack list and add them to 'states_' of the base class by calling 'add_state' from the base class
    }
};

#endif // MACHINE_ONE_H_
