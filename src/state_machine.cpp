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

StateMachine::StateMachine(Arduino_GFX *gfx_ptr)
{
    state = StateMachine::State::IDLE;
    if (gfx_ptr == NULL)
    {
        debug("Error: gfx_ptr is NULL\n");
        return;
    }
    gfx = gfx_ptr;
    debug("State machine initialized\n");
};

void StateMachine::change_state(State new_state)
{
    previous_state = state;
    state = new_state;
    last_state_change_ms = get_time();
    debug("State changed from %s to %s at %d\n", get_state_string(previous_state), get_state_string(), last_state_change_ms);
    debug("Clear display\n");
    gfx->fillScreen(BLACK);
};

void StateMachine::update_state_machine(lelit_data_t *new_data)
{
    info("Updating state machine, current state: %s\n", get_state_string());
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
};

void StateMachine::draw_state(int x, int y, bool debug)
{
    int time_brewing = get_time_in_current_state();

    switch (state)
    {
    case StateMachine::State::BREWING:
        time_brewing = get_time_in_current_state();
        draw_state_brewing(x, y, time_brewing, debug);
        break;
    case StateMachine::State::IDLE:
        if (previous_state == StateMachine::State::BREWING && get_time_in_current_state() < 5000)
        {
            draw_state_brewing(x, y, last_brewing_time_ms, debug);
            draw_state_default(x, y + 100, debug);
            break;
        }
        else
        {
            // intentional fallthrough
            ;
        }
    default:
        draw_state_default(x, y, debug);
        break;
    }
};

void StateMachine::draw_state_default(int x, int y, bool debug)
{
    gfx->setTextSize(1);
    gfx->setCursor(x, y);
    gfx->setTextColor(WHITE, BLACK);

    gfx->printf("   Mode: %6s\n", last_data.mode == COFFEE ? "Coffee" : "Steam");
    gfx->printf("   Steam:   %3d/%3d", last_data.steam_temp, last_data.target_steam_temp);
    // Use special functionality to write degree symbol
    gfx->write(0xF8);
    gfx->printf("C\n");
    gfx->printf("   Coffee:  %3d", last_data.coffee_temp);
    gfx->write(0xF8);
    gfx->printf("C\n");
    gfx->printf("   Boost:   %3ds\n", last_data.boost_countdown);
    gfx->printf("   Heating: %3d\n", last_data.heating_element_on);
    gfx->printf("   Pump:    %3d\n", last_data.pump_on);

    gfx->printf("   State: %s\n", get_state_string());
    gfx->printf("   Time in state: %6d\n", get_time_in_current_state());
}

void StateMachine::draw_state_brewing(int x, int y, int time_brewing, bool debug)
{
    gfx->setTextSize(3);
    gfx->setCursor(x, y);

    gfx->setTextColor(WHITE, BLACK);
    gfx->printf("Brewing since\n");
    gfx->setCursor(100, 100);
    gfx->setTextColor(RED, BLACK);
    gfx->setTextSize(5);
    gfx->printf("%2.1fs", time_brewing / 1000.0);
}

const char *StateMachine::get_state_string(StateMachine::State state)
{
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

int StateMachine::get_time_in_current_state()
{
    return get_time() - last_state_change_ms;
};