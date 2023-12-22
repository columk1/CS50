#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

const int HEADER_SIZE = sizeof(WAVHEADER);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }
    // Check for correct file extension
    if (strcmp(argv[1] + strlen(argv[1]) - 4, ".wav"))
    {
        printf("Input is not a WAV file.\n");
        return 1;
    }

    // Open input file for reading
    FILE *input_wav = fopen(argv[1], "r");
    if (input_wav == NULL)
    {
        printf("No such file found.\n");
        return 1;
    }

    // Read header
    WAVHEADER header;
    fread(&header, HEADER_SIZE, 1, input_wav);

    // Use check_format to ensure WAV format
    if (!check_format(header))
    {
        printf("Input is not a valid WAV file.\n");
        return 1;
    }

    // Open output file for writing
    FILE *output_wav = fopen(argv[2], "w");
    if (output_wav == NULL)
    {
        printf("Failed to open output file.\n");
        return 1;
    }

    // Write header to file
    fwrite(&header, HEADER_SIZE, 1, output_wav);

    // Use get_block_size to calculate size of block
    int block_size = get_block_size(header);
    printf("%i\n", block_size);


    // Write reversed audio to file
    uint8_t buffer[block_size];
    fseek(input_wav, 0, SEEK_END);

    while (ftell(input_wav) > HEADER_SIZE)
    {
        fseek(input_wav, -block_size, SEEK_CUR);
        fread(buffer, block_size, 1, input_wav);
        fwrite(buffer, block_size, 1, output_wav);
        fseek(input_wav, -block_size, SEEK_CUR);
    }
    fclose(input_wav);
    fclose(output_wav);

    return 0;
}

int check_format(WAVHEADER header)
{
    if (!memcmp(header.format, "WAVE", 4))
    {
        return 1;
    }
    return 0;
}

int get_block_size(WAVHEADER header)
{
    return header.numChannels * header.bitsPerSample * 0.125;
}