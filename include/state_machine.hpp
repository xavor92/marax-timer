#pragma once

#include <Arduino_GFX_Library.h>

#include "decoder.hpp"

class StateMachine
{
public:
    StateMachine(Arduino_GFX *gfx);
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
    void draw_state(int x, int y, bool debug);
    void update_state_machine(lelit_data_t *new_data);

private:
    State state;
    State previous_state;
    lelit_data_t last_data;
    const int heating_threshold = 10;
    const int coffee_temp_target = 93;
    int last_state_change_ms;
    int last_brewing_time_ms = 0;
    Arduino_GFX *gfx;

    void draw_state_default(int x, int y, bool debug);
    void draw_state_brewing(int x, int y, int time_brewing, bool debug);
    void change_state(State new_state);
};
