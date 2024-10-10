#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "state_machine.hpp"

#define DEBUG true
#define debug(...) if(DEBUG) { printf(__VA_ARGS__); } else {};

#define INFO false
#define info(...) if(INFO) { printf(__VA_ARGS__); } else {};



long get_time()
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
}

StateMachine::StateMachine()
{
    state = StateMachine::State::IDLE;
    debug("State machine initialized\n");
};

void StateMachine::change_state(State new_state)
{
    previous_state = state;
    state = new_state;
    last_state_change_ms = get_time();
    debug("State changed from %s to %s at %d\n", get_state_string(previous_state), get_state_string(), last_state_change_ms);
};

bool StateMachine::update_state_machine(lelit_data_t *new_data)
{
    info("Updating state machine, current state: %s\n", get_state_string());
    StateMachine::State starting_state = state;
    memcpy(&last_data, new_data, sizeof(lelit_data_t));
    switch (state)
    {
    case StateMachine::State::IDLE:
        if (last_data.pump_on)
        {
            change_state(StateMachine::State::BREWING);
        }
        else if (last_data.target_steam_temp < 100)
        {
            change_state(StateMachine::State::ERROR);
        }
        else if (
            (abs(last_data.target_steam_temp - last_data.steam_temp) > heating_threshold) ||
            (abs(last_data.coffee_temp - coffee_temp_target) > heating_threshold))
        {
            change_state(StateMachine::State::HEATING);
        }
        break;
    case StateMachine::State::HEATING:
        if (last_data.pump_on)
        {
            change_state(StateMachine::State::BREWING);
        }
        else if (last_data.target_steam_temp < 100)
        {
            change_state(StateMachine::State::ERROR);
        }
        else if (last_data.heating_element_on == false)
        {
            change_state(StateMachine::State::IDLE);
        }
        break;
    case StateMachine::State::ERROR:
        if (last_data.target_steam_temp > 100)
        {
            change_state(StateMachine::State::IDLE);
        }
        break;
    case StateMachine::State::BREWING:
        if (last_data.pump_on == false)
        {
            // Store brewing time to display in idle for a few more seconds
            last_brewing_time_ms = get_time_in_current_state();
            change_state(StateMachine::State::IDLE);
        }
        break;
    default:
        break;
    }
    // check if state was changed
    info("Leaving state machine, old state: %d current state: %d\n", starting_state, state);
    return starting_state != state;
};

const char *StateMachine::get_state_string(StateMachine::State state)
{
    switch (state)
    {
    case StateMachine::State::IDLE:
        return "IDLE   ";
    case StateMachine::State::HEATING:
        return "HEATING";
    case StateMachine::State::ERROR:
        return "ERROR  ";
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

int StateMachine::get_time_in_current_state()
{
    return get_time() - last_state_change_ms;
};