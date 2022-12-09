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


uint8_t fetch_byte();
uint8_t read_byte(uint16_t address);

// Decrement the given 8-bit value and update the flags
void dec8(uint8_t *value);

// Decrement a 16-bit value
void dec16(uint16_t *value);

// Increment the given 8-bit value and update the flags
void inc(uint8_t *value);

// Rotate the given 8-bit value left and update the flags
uint8_t rlc(uint8_t value);

uint8_t rrc(uint8_t value);
// Fetch a 16-bit word from the gameboy's memory
uint16_t fetch_word();

// Write a byte to the gameboy's memory
void write_byte(uint16_t address, uint8_t value);

// Add the values of the HL and BC registers
void add_hl(uint16_t* hl, uint16_t* bc);
#endif
