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

    void update_state_machine(lelit_data_t *data);
    void update_display();

private:
    State state;
    const int heating_threshold = 10;
    const int coffee_temp_target = 93;

    const char* get_state_string();
    const char* get_state_string(StateMachine::State state);
};
