#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "decoder.hpp"

#define DEBUG true

#define debug(...) if(DEBUG) { printf(__VA_ARGS__); } else {};


uint8_t expected_header[HEADER_SIZE] = {0x5E, 0x67, 0xB4};

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
    debug("Swapping buffer to %d\n", new_buffer_index);
    active_buffer_index = new_buffer_index;
    current_buffer_pos = buffer[active_buffer_index];
    memset(current_buffer_pos, 0, BUFFER_SIZE);
}

void Decoder::handle_bytes(uint8_t byte)
{
    // write byte into buffer
    *current_buffer_pos = byte;

    // If a nullbyte was received, this might be a packet termination
    if (*current_buffer_pos == '\0')
    {
        // TODO: validate packet
        if (parse_active_buffer()) // assume valid packet
        {
            printf("Got a valid packet\n");
        } else {
            // This might be just the initial data, so this might be expected
            printf("Got an invalid packet\n");
        }
        dump_active_buffer();
        swap_buffer();
    } else {
        // If we filled a buffer without receiving a null byte, we just swap to the
        // next buffer and start over
        if (current_buffer_pos == buffer[active_buffer_index] + BUFFER_SIZE - 1)
        {
            swap_buffer();
        } else {
            current_buffer_pos++;
        }
    }
}


bool Decoder::validate_data()
{
    bool valid_data = true;
    for (int i = 0; i < sizeof(data.header) / sizeof(data.header[0]); i++)
    {
        if (data.header[i] != expected_header[i])
        {
            debug("Error: invalid header\n");
            valid_data = false;
        }
    }

    return valid_data;
}

bool Decoder::parse_active_buffer()
{
    uint8_t *buffer_ptr = buffer[active_buffer_index];
    memset(&data, 0, sizeof(data));
    for (int i = 0; i < sizeof(data.header) / sizeof(data.header[0]); i++)
    {
        data.header[i] = buffer_ptr[i];
    }

    return validate_data();
}

void Decoder::dump_active_buffer()
{
    uint8_t *buffer_ptr = this->buffer[active_buffer_index];
    printf("Buffer: ");
    for (int i = 0; i < BUFFER_SIZE && buffer_ptr[i] != 0; i++)
    {
        printf("%02X ", buffer_ptr[i]);
    }
    printf("\n");
};
