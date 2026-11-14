#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check for correct usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open memory card file
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    BYTE buffer[512];
    FILE *output = NULL;
    int file_count = 0;
    char filename[8]; // "000.jpg\0"

    // Read 512 bytes at a time
    while (fread(buffer, sizeof(BYTE), 512, input) == 512)
    {
        // Check for start of a new JPEG
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // If already writing a JPEG, close it
            if (output != NULL)
            {
                fclose(output);
            }

            // Create new filename
            sprintf(filename, "%03i.jpg", file_count);
            output = fopen(filename, "w");
            file_count++;
        }

        // If we're currently writing to a JPEG, write the buffer
        if (output != NULL)
        {
            fwrite(buffer, sizeof(BYTE), 512, output);
        }
    }

    // Close remaining files
    if (output != NULL)
    {
        fclose(output);
    }

    fclose(input);
    return 0;
}
