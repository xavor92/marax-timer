#include <stdio.h>
#include <stdint.h>

#include "decoder.hpp"

#define FILE_BUFFER_SIZE 1024 * 1024

int read_file(const char* filename, uint8_t *buffer, size_t buffer_size, int *file_size)
{
    *file_size = 0;
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error: Could not open file %s\n", filename);
        return -1;
    }

    size_t read_size = fread(buffer, 1, buffer_size, file);
    if (read_size == 0)
    {
        printf("Error: Could not read file %s\n", filename);
        return -2;
    }
    fclose(file);

    *file_size = read_size;
    return 0;
}

int main()
{
    uint8_t * buffer = new unsigned char[FILE_BUFFER_SIZE];
    int file_size = 0;
    printf("Hello World from Decoder!\n");
    int result = read_file("working.raw", buffer, FILE_BUFFER_SIZE, &file_size);

    printf("Result: %d\n", result);
    printf("File size: %d\n", file_size);

    Decoder decoder;
    for (int i = 0; i < file_size; i++)
    {
        decoder.handle_bytes(buffer[i]);
    }

    return 0;
}
