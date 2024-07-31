#include <stdint.h>

/*
 * from: https://www.kaffee-netz.de/threads/lelit-mara-x-shot-timer.148272/#post-2160153
 *
 * Example Data: C1.06,116,124,093,0840,1,0\n 
 *    [Pos] [Data] [Describtion]
 *    0)      C     Coffee Mode (C) or SteamMode (V)  -  1.06  Software Version
 *    1)      116   current steam temperature (Celsisus)
 *    2)      124   target steam temperature (Celsisus)
 *    3)      093   current coffee temperature (Celsisus)
 *    4)      0840  countdown for 'boost-mode'
 *    5)      1     heating element on or off
 *    6)      0     pump on or off
 */

#define INPUT_BUFFER_COUNT 2
#define BUFFER_SIZE 32
#define HEADER_SIZE 3

enum mode {
    COFFEE = 'C',
    STEAM = 'V'
} typedef machine_mode_t;

struct {
    machine_mode_t mode;
    /* Skipping version */
    long int steam_temp;
    long int target_steam_temp;
    long int coffee_temp;
    long int boost_countdown;
    bool heating_element_on;
    bool pump_on;
} typedef lelit_data_t;

class Decoder
{
public:
    Decoder();
    ~Decoder() {};
    void handle_bytes(uint8_t byte);
    lelit_data_t data;
private:
    void dump_active_buffer();
    void swap_buffer();
    bool parse_active_buffer();
    bool extract_int(char *start_ptr, char end_char, char** end_char_ptr, long int *value);
    void print_current_data();

    uint8_t active_buffer_index;
    uint8_t *current_buffer_pos;
    uint8_t buffer[INPUT_BUFFER_COUNT][BUFFER_SIZE];
};