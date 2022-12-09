#ifndef GB_H
#define GB_H

#include <stdlib.h>

// Gameboy types
#define GB_TYPE_GB 0
#define GB_TYPE_GBC 1

// Load the gameboy ROM
int load_rom(FILE *rom_file);

// Initialize the gameboy emulator
int init_gb();

// Run the gameboy emulator
void run_gb();

#endif
