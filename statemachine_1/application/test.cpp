#include "machine_one.h"
#include "states.h"
#include <iostream> // include necessary header files

int main() {
    // Instantiate the derived class 'State machine' with the three classes StateA, StateB and StateC, in a variable sm
    MachineOne<StateA, StateB, StateC> sm;

    // Call sm's 'set_current_state' with argument 'A'
    sm.set_current_state("A");

    // Call sm's 'execute' 7 times
    for (int i = 0; i < 7; i++) {
        sm.execute();
    }

    return 0;
}
