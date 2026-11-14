// volume.c

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// 16-bit signed integer type (audio sample)
typedef int16_t sample;

int main(int argc, char *argv[])
{
    // Check for correct command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Get command-line arguments
    char *input_file = argv[1];
    char *output_file = argv[2];
    float factor = atof(argv[3]);

    // Open input file
    FILE *input = fopen(input_file, "rb");
    if (input == NULL)
    {
        printf("Could not open %s.\n", input_file);
        return 1;
    }

    // Open output file
    FILE *output = fopen(output_file, "wb");
    if (output == NULL)
    {
        fclose(input);
        printf("Could not create %s.\n", output_file);
        return 1;
    }

    // Copy WAV header (first 44 bytes)
    uint8_t header[44];
    fread(header, sizeof(uint8_t), 44, input);
    fwrite(header, sizeof(uint8_t), 44, output);

    // Read samples from input, multiply by factor, and write to output
    sample buffer;
    while (fread(&buffer, sizeof(sample), 1, input))
    {
        buffer = buffer * factor;
        fwrite(&buffer, sizeof(sample), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);

    return 0;
}
