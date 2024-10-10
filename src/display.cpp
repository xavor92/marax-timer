#include "display.hpp"
#include "state_machine.hpp"

#define DEBUG true
#define debug(...) if(DEBUG) { printf(__VA_ARGS__); } else {};

#define INFO false
#define info(...) if(INFO) { printf(__VA_ARGS__); } else {};


Display::Display(Arduino_GFX *gfx_ptr)
{
    if (gfx_ptr == NULL)
    {
        debug("Error: gfx_ptr is NULL\n");
        return;
    }
    gfx = gfx_ptr;
}

void Display::draw_state(StateMachine &state_machine)
{
    int time_brewing = state_machine.get_time_in_current_state();

    int x = 100;
    int y = 100;
    bool debug = true;
    StateMachine::State state = state_machine.state;
    switch (state)
    {
    // TODO: Reenable state drawing
    /*case StateMachine::State::BREWING:
        time_brewing = state_machine.get_time_in_current_state();
        draw_state_brewing(x, y, time_brewing, debug);
        break;
    case StateMachine::State::IDLE:
        if (state_machine.previous_state == StateMachine::State::BREWING && state_machine.get_time_in_current_state() < 5000)
        {
            draw_state_brewing(x, y, state_machine.last_brewing_time_ms, debug);
            draw_state_default(state_machine, x, y + 100, debug);
            break;
        }
        else
        {
            // intentional fallthrough
            ;
        }*/
    default:
        draw_state_default(state_machine, x, y, debug);
        break;
    }
}

void Display::draw_state_default(StateMachine &state, int x, int y, bool debug)
{
    gfx->setTextSize(1);
    gfx->setCursor(x, y);
    gfx->setTextColor(WHITE, BLACK);

    gfx->printf("   Mode: %6s\n", state.last_data.mode == COFFEE ? "Coffee" : "Steam");
    gfx->printf("   Steam:   %3d/%3d", state.last_data.steam_temp, state.last_data.target_steam_temp);
    // Use special functionality to write degree symbol
    gfx->write(0xF8);
    gfx->printf("C\n");
    gfx->printf("   Coffee:  %3d", state.last_data.coffee_temp);
    gfx->write(0xF8);
    gfx->printf("C\n");
    gfx->printf("   Boost:   %3ds\n", state.last_data.boost_countdown);
    gfx->printf("   Heating: %3d\n", state.last_data.heating_element_on);
    gfx->printf("   Pump:    %3d\n", state.last_data.pump_on);

    gfx->printf("   State: %s\n", state.get_state_string());
    gfx->printf("   Time in state: %6d\n", state.get_time_in_current_state());
}

void Display::draw_state_brewing(int x, int y, int time_brewing, bool debug)
{
    gfx->setTextSize(3);
    gfx->setCursor(x, y);

    gfx->setTextColor(WHITE, BLACK);
    gfx->printf("Brewing since\n");
    gfx->setCursor(100, 100);
    gfx->setTextColor(RED, BLACK);
    gfx->setTextSize(5);
    gfx->printf("%2.1fs", time_brewing / 1000.0);
    ;
}

