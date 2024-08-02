#include <stdio.h>
#include <stdlib.h>

#include "state_machine.hpp"

#define DEBUG true
#define debug(...) if(DEBUG) { printf(__VA_ARGS__); } else {};

#define INFO false
#define info(...) if(INFO) { printf(__VA_ARGS__); } else {};

StateMachine::StateMachine() {
    state = StateMachine::State::IDLE;
    debug("State machine initialized\n");
};

void StateMachine::update_state_machine(lelit_data_t *data)
{
    info("Updating state machine, current state: %s\n", get_state_string());
    StateMachine::State old_state = state;
    switch (state)
    {
    case StateMachine::State::IDLE:
        if (data->pump_on)
        {
            state = StateMachine::State::BREWING;
        }
        else if (data->target_steam_temp < 100)
        {
            state = StateMachine::State::ERROR;
        } else if (
            (abs(data->target_steam_temp - data->steam_temp) > heating_threshold) || 
            (abs(data->coffee_temp - coffee_temp_target) > heating_threshold)
            )
        {
            state = StateMachine::State::HEATING;
        }
        break;
    case StateMachine::State::HEATING:
        if (data->pump_on)
        {
            state = StateMachine::State::BREWING;
        }
        else if (data->target_steam_temp < 100)
        {
            state = StateMachine::State::ERROR;
        } else if (data->heating_element_on == false)
        {
            state = StateMachine::State::IDLE;
        }
        break;
    case StateMachine::State::ERROR:
        if (data->target_steam_temp > 100)
        {
            state = StateMachine::State::IDLE;
        }
        break;
    case StateMachine::State::BREWING:
        if (data->pump_on == false)
        {
            state = StateMachine::State::IDLE;
        }
        break;
    default:
        break;
    }
    if (old_state != state)
    {
        debug("State changed from %s to %s\n", get_state_string(old_state), get_state_string());
    }
};

const char* StateMachine::get_state_string(StateMachine::State state) {
    switch (state)
    {
    case StateMachine::State::IDLE:
        return "IDLE";
    case StateMachine::State::HEATING:
        return "HEATING";
    case StateMachine::State::ERROR:
        return "ERROR";
    case StateMachine::State::BREWING:
        return "BREWING";
    default:
        return "UNKNOWN";
    };
};

const char *StateMachine::get_state_string()
{
    return get_state_string(state);
};
