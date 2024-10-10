#pragma once

#include "decoder.hpp"

class StateMachine
{
public:
    StateMachine();
    ~StateMachine() {};

    enum class State {
        IDLE,
        HEATING,
        ERROR,
        BREWING,
    };

    const char* get_state_string();
    const char* get_state_string(StateMachine::State state);
    int get_time_in_current_state();
    bool update_state_machine(lelit_data_t *new_data);

// TODO: Redefine as private where it applies
//private:
    lelit_data_t last_data;
    State state;
    State previous_state;
    const int heating_threshold = 10;
    const int coffee_temp_target = 93;
    int last_state_change_ms;
    int last_brewing_time_ms = 0;

    void change_state(State new_state);
};
