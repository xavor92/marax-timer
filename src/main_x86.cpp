#include <stdint.h>
#include <stdio.h>

#include "decoder.hpp"
#include "example_data.hpp"

int main()
{
    char *example_data_ptr = (char *)example_data;
    char rc = '\0';
    int i = 0;
    int file_size = 0;
    printf("Hello World from %s\n", __FILE__);

    Decoder decoder;

    while (true)
    {
        rc = *example_data_ptr;
        example_data_ptr++;
        if (*example_data_ptr == 0)
        {
            printf("Reached end of buffer\n");
            break;
        }

        // we do not have serial data yet, so iterate over example data
        bool updated = decoder.handle_bytes(rc);

        if (updated)
        {
            i++;
            decoder.print_current_data();
        }
    }

    return 0;
}
