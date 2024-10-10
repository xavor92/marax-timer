#pragma once

#include <Arduino_GFX_Library.h>

#include "state_machine.hpp"


class Display
{
public:
    Display(Arduino_GFX *gfx_ptr);
    ~Display() {};

    void draw_state(StateMachine &state);

private:
    Arduino_GFX *gfx;
    void draw_state_default(StateMachine &state, int x, int y, bool debug);
    void draw_state_brewing(int x, int y, int time_brewing, bool debug);
};
