#ifndef MACHINE_BASE_H_
#define MACHINE_BASE_H_

#include <string>
#include <map>
#include "state_base.h" // include necessary header file

class MachineBase {
private:
    std::map<std::string, StateBase*> states_; // private map of states
    StateBase* current_state_; // protected current state

protected:
    std::string name_; // protected property 'name'

    // protected function to add a state to the 'states_' map
    int add_state(StateBase* state);
    //  {
    //     states_.insert(std::make_pair(state->get_name(), state));
    //     return 0; // return 0 on success
    // }

public:
    MachineBase(std::string name); // constructor with a string argument 'name'
    virtual ~MachineBase(); // virtual destructor

    std::string get_name() const; // getter function for 'name_'
    std::map<std::string, StateBase*> get_states() const; // getter function for 'states_'
    StateBase* get_current_state() const; // getter function for 'current_state_'

    int set_current_state(std::string state_name); // public method to set the current state
//    virtual int execute() = 0; // pure virtual function for executing the machine
    int execute(); // pure virtual function for executing the machine
};

#endif // MACHINE_BASE_H_
