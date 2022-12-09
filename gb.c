#include <stdio.h>
#include <stdint.h>
#include "gb.h"

// Gameboy memory sizes
const int GB_ROM_SIZE = 0x8000;
const int GB_VRAM_SIZE = 0x2000;
const int GB_WRAM_SIZE = 0x2000;
const int GB_HRAM_SIZE = 0x80;

// Gameboy color memory sizes
const int GBC_ROM_SIZE = 0x100000;
const int GBC_VRAM_SIZE = 0x4000;
const int GBC_WRAM_SIZE = 0x8000;
const int GBC_HRAM_SIZE = 0x80;

// The gameboy ROM
uint8_t *rom;

// The gameboy memory
uint8_t *vram;
uint8_t *wram;
uint8_t *hram;

// The gameboy registers
uint16_t pc;
uint16_t sp;
uint8_t a;
uint8_t b;
uint8_t c;
uint8_t d;
uint8_t e;
uint8_t h;
uint8_t l;
uint8_t flags;

// The gameboy type (GB or GBC)
int gb_type;

// Load the gameboy ROM
int load_rom(FILE *rom_file)
{
    // Get the size of the ROM file
    fseek(rom_file, 0, SEEK_END);
    long rom_size = ftell(rom_file);
    fseek(rom_file, 0, SEEK_SET);

    // Allocate memory for the ROM
    rom = (uint8_t *)malloc(rom_size);
    if (!rom)
    {
        printf("Error: Failed to allocate memory for ROM.\n");
        return 1;
    }

    // Read the ROM data into memory
    if (fread(rom, 1, rom_size, rom_file) != rom_size)
    {
        printf("Error: Failed to read ROM data.\n");
        return 1;
    }

    // Determine the gameboy type based on the ROM header
    gb_type = (rom[0x143] == 0x80 || rom[0x143] == 0xC0) ? GB_TYPE_GBC : GB_TYPE_GB;

    return 0;
}

// Initialize the gameboy emulator
int init_gb()
{
    // Allocate memory for the gameboy memory
    if (gb_type == GB_TYPE_GB)
    {
        vram = (uint8_t *)malloc(GB_VRAM_SIZE);
        wram = (uint8_t *)malloc(GB_WRAM_SIZE);
        hram = (uint8_t *)malloc(GB_HRAM_SIZE);
    }
    else
    {
        vram = (uint8_t *)malloc(GBC_VRAM_SIZE);
        wram = (uint8_t *)malloc(GBC_WRAM_SIZE);
        hram = (uint8_t *)malloc(GBC_HRAM_SIZE);
    }

    if (!vram || !wram || !hram)
    {
        printf("Error: Failed to allocate memory for gameboy memory.\n");
        return 1;
    }

    // Initialize the gameboy registers
    pc = 0x100;
    sp = 0xFFFE;
    a = 0x01;
    b = 0x00;
    c = 0x13;
    d = 0x00;
    e = 0xD8;
    h = 0x01;
    l = 0x4D;
    flags = 0xB0;

    return 0;
}

// The number of clock cycles per frame
const int CYCLES_PER_FRAME = 4194304 / 60;

// The number of cycles since the last frame
int cycles_since_last_frame = 0;

// Run the gameboy emulator
void run_gb()
{
    // Fetch and execute instructions until a frame is complete
    while (cycles_since_last_frame < CYCLES_PER_FRAME)
    {
        // TODO: Fetch and execute the next instruction

        // TODO: Handle interrupts and other hardware events

        // Increment the cycle counter
        cycles_since_last_frame++;
    }

    // TODO: Update the video and audio output

    // Reset the cycle counter
    cycles_since_last_frame = 0;
}
