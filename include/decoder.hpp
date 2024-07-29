#include <stdint.h>

#define INPUT_BUFFER_COUNT 2
#define BUFFER_SIZE 32
#define HEADER_SIZE 3

struct {
    uint8_t header[HEADER_SIZE];
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
    bool validate_data();

    uint8_t active_buffer_index;
    uint8_t *current_buffer_pos;
    uint8_t buffer[INPUT_BUFFER_COUNT][BUFFER_SIZE];
};