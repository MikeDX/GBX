#include <stdio.h>
#include <stdlib.h>
#include "window.h"
#include "gb.h"

int main(int argc, char *argv[])
{
    // Check if a gameboy ROM file was provided as an argument
    if (argc < 2)
    {
        printf("Error: No gameboy ROM file provided.\n");
        return 1;
    }

    // Create the SDL window and renderer
    if (create_window() != 0)
        return 1;

    // Load the gameboy ROM file
    FILE *rom_file = fopen(argv[1], "rb");
    if (!rom_file)
    {
        printf("Error: Failed to open gameboy ROM file.\n");
        return 1;
    }

    // TODO: Read the ROM file and initialize the emulator

    // TODO: Run the emulator

    // Close the ROM file and exit
    fclose(rom_file);

    // Destroy the SDL window and renderer
    destroy_window();
    return 0;
}
