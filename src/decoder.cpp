#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "decoder.hpp"

#define DEBUG false
#define debug(...) if(DEBUG) { printf(__VA_ARGS__); } else {};

#define INFO false
#define info(...) if(INFO) { printf(__VA_ARGS__); } else {};

Decoder::Decoder()
{
    active_buffer_index = 0;
    current_buffer_pos = buffer[active_buffer_index];

    // clear buffers
    for (int i = 0; i < INPUT_BUFFER_COUNT; i++)
    {
        memset(buffer[i], 0, BUFFER_SIZE);
    }
};

void Decoder::swap_buffer() {
    int new_buffer_index = (active_buffer_index + 1) % INPUT_BUFFER_COUNT;
    info("Swapping buffer to %d\n", new_buffer_index);
    active_buffer_index = new_buffer_index;
    current_buffer_pos = buffer[active_buffer_index];
    memset(current_buffer_pos, 0, BUFFER_SIZE);
}

bool Decoder::handle_bytes(char byte)
{
    bool updated_data = false;
    // write byte into buffer
    *current_buffer_pos = byte;

    // If a linebreak was received, this might be a packet termination
    if (*current_buffer_pos == '\n')
    {
        // add null termination to handle as string
        info("Received linebreak\n");
        *current_buffer_pos = 0;
        info("Buffer: ");
        info("%s\n", buffer[active_buffer_index]);
        if (parse_active_buffer())
        {
            debug("Got a valid packet\n");
            updated_data = true;
            if (DEBUG) {
                print_current_data();
            }
        } else {
            // This might be just the initial data, so this might be expected
            debug("Got an invalid packet\n");
        }
        swap_buffer();
    } else {
        // If a buffer minus the space for a terminating nullbyte was filled
        // without receiving a linebreak, we still swap the buffer and wait
        // for the next packet
        if (current_buffer_pos == buffer[active_buffer_index] + BUFFER_SIZE - 1)
        {
            swap_buffer();
        } else {
            current_buffer_pos++;
        }
    }
    return updated_data;
}


bool Decoder::extract_int(char *start_ptr, char end_char, char** end_char_ptr, long int *value)
{
    char *int_end_ptr;
    *end_char_ptr = strchr(start_ptr, end_char);
    if (*end_char_ptr == NULL)
    {
        printf("Could not find end char %c\n", end_char);
        return false;
    }
    *value = strtol(start_ptr, &int_end_ptr, 10);
    if (int_end_ptr != *end_char_ptr)
    {
        printf("Extra chars between end of int and separator\n");
        return false;
    }
    return true;
}

bool Decoder::parse_active_buffer()
{
    const char separator = ',';
    bool ret = false;
    char *element_start, *element_end;
    long int value;
    char *buffer_ptr = buffer[active_buffer_index];
    memset(&data, 0, sizeof(data));

    // buffer is expected to be null terminated, but might have trailing cr lf chars.
    element_end = buffer_ptr + strlen(buffer_ptr) - 1;
    while( *element_end == '\r' || *element_end == '\n')
    {
        info("Removing trailing CR or LF\n");
        *(element_end) = '\00';
        element_end--;
    }

    // First char must be either 'C' or 'V', otherwise we discard the packet
    if (buffer_ptr[0] == 'C') {
        data.mode = COFFEE;
    } else if (buffer_ptr[0] == 'V') {
        data.mode = STEAM;
    } else {
        return false;
    }
    debug("Mode: %s, ", data.mode == COFFEE ? "Coffee" : "Steam");

    // get and print major and minor version
    ret = extract_int(buffer_ptr + 1, '.', &element_end, &value);
    if (!ret) {
        printf("Could not extract version\n");
        return false;
    }
    debug("Version: %ld.", value);
    ret = extract_int((element_end + 1), ',', &element_end, &value);
    if (!ret) {
        printf("Could not extract version\n");
        return false;
    }
    debug("%ld, ", value);

    // get and print steam temperature
    ret = extract_int(element_end + 1, separator, &element_end, &data.steam_temp);
    if (!ret) {
        printf("Could not extract steam temperature\n");
        return false;
    }
    debug("Steam Temp: %d, ", data.steam_temp);

    //TODO: Validate if this is correct
    // Current target temperature has a leading O, remove it
    if (*(element_end + 1) == 'O') {
        info("Skipping leading O\n");
        element_end++;
    } else {
        info("No leading O\n");
    }

    // get and print target steam temperature
    ret = extract_int((element_end + 1), separator, &element_end, &data.target_steam_temp);
    if (!ret) {
        debug("Could not extract target steam temperature\n");
        return false;
    }
    debug("Target Steam Temp: %d, ", data.target_steam_temp);

    // get and print coffee temperature
    ret = extract_int((element_end + 1), separator, &element_end, &data.coffee_temp);
    if (!ret) {
        debug("Could not extract coffee temperature\n");
        return false;
    }
    debug("Coffee Temp: %d, ", data.coffee_temp);

    // get and print boost countdown
    ret = extract_int(element_end + 1, separator, &element_end, &data.boost_countdown);
    if (!ret) {
        debug("Could not extract boost countdown\n");
        return false;
    }
    debug("Boost Countdown: %d, ", data.boost_countdown);

    // get and print heating element on/off
    ret = extract_int((element_end + 1), separator, &element_end, &value);
    if (!ret) {
        debug("Could not extract heating element on/off\n");
        return false;
    }
    data.heating_element_on = (bool)value;
    debug("Heating Element: %d, ", data.heating_element_on);

    // get and print pump on/off
    ret = extract_int((element_end + 1), '\00', &element_end, &value);
    if (!ret) {
        debug("Could not extract pump on/off\n");
        return false;
    }
    data.pump_on = (bool)value;
    debug("Pump: %d\n", data.pump_on);

    return true;
}

void Decoder::print_current_data(void)
{
    printf("Mode: %s, ", data.mode == COFFEE ? "Coffee" : "Steam");
    printf("Steam Temp: %d, ", data.steam_temp);
    printf("Target Steam Temp: %d, ", data.target_steam_temp);
    printf("Coffee Temp: %d, ", data.coffee_temp);
    printf("Boost Countdown: %d, ", data.boost_countdown);
    printf("Heating Element: %d, ", data.heating_element_on);
    printf("Pump: %d\n", data.pump_on);
}



void Decoder::dump_active_buffer()
{
    char *buffer_ptr = this->buffer[active_buffer_index];
    printf("Buffer: ");
    for (int i = 0; i < BUFFER_SIZE && buffer_ptr[i] != 0; i++)
    {
        printf("%02X ", buffer_ptr[i]);
    }
    printf("\n");
};
