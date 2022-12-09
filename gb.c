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

// Declare the memory array with a size of 65536 bytes to cover the entire gameboy address space
uint8_t memory[65536];

// Declare a structure to hold the gameboy's CPU registers
// Define the CPU struct
typedef struct 
{
	// Word registers
	union
	{
		uint16_t af, bc, de, hl, ix, iy, sp;
	};
	
	// Byte registers
	union
	{
		uint8_t f, a, c, b, e, d, l, h, ixl, ixh, iyl, iyh;
	};
} CPU;

// Declare a global variable to hold the gameboy's CPU
CPU cpu;

// Declare constants to represent the different flags in the gameboy's CPU
#define Z_FLAG 0x80
#define N_FLAG 0x40
#define H_FLAG 0x20
#define C_FLAG 0x10

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
    // Fetch the next instruction
    uint8_t opcode = fetch_byte();
    static uint16_t address = 0x0;

    // Decode and execute the instruction
    switch (opcode)
    {

        // NOP (No Operation)
        case 0x00:
            break;

        // LD BC, d16 (Load 16-bit immediate value into BC)
        case 0x01:
            cpu.bc = fetch_word();
            break;

        // LD (BC), A (Load A into memory at address in BC)
        case 0x02:
            write_byte(cpu.bc, a);
            break;

        // INC BC (Increment BC)
        case 0x03:
            inc16(&cpu.bc);
            break;

        // INC B (Increment B)
        case 0x04:
            inc8(&cpu.b);
            break;

        // DEC B
        case 0x05:
            dec8(&cpu.b);
            break;
    
        // LD B,d8
        case 0x06:
            cpu.b = fetch_byte();
            break;

        // RLCA
        case 0x07:
            cpu.a = rlc(cpu.a);
            break;

        // LD (a16),SP
        case 0x08:
            address = fetch_word();
            write_byte(address, cpu.sp & 0xFF);
            write_byte(address + 1, cpu.sp >> 8);
            break;

        // ADD HL,BC
        case 0x09:
            add_hl(&cpu.hl, &cpu.bc);
            break;

        // LD A,(BC)
        case 0x0A:
            cpu.a = read_byte(cpu.bc);
            break;

        // DEC BC
        case 0x0B:
            dec16(&cpu.bc);
            break;

        // INC C
        case 0x0C:
            inc(&cpu.c);
            break;

        // DEC C
        case 0x0D:
            dec8(&cpu.c);
            break;

        // LD C,d8
        case 0x0E:
            cpu.c = fetch_byte();
            break;

        // RRCA
        case 0x0F:
            cpu.a = rrc(cpu.a);
            break;
        default:
                printf("Unknown opcode  0x%02x\n", opcode);
                break;           
    }
        
    // TODO: Fetch and execute the next instruction

    // TODO: Handle interrupts and other hardware events

    // Increment the cycle counter
    cycles_since_last_frame++;

    // TODO: Update the video and audio output

    // Reset the cycle counter
    cycles_since_last_frame = 0;
}


uint8_t fetch_byte()
{
    // Read the next byte from memory at the PC address
    uint8_t data = read_byte(pc);

    // Increment the PC to point to the next instruction
    pc++;

    return data;
}

uint8_t read_byte(uint16_t address)
{
    // Read the byte from memory at the given address
    uint8_t data = memory[address];

    return data;
}

// Set the given flag in the gameboy's CPU
void set_flag(uint8_t flag)
{
    cpu.f |= flag;
}

// Clear the given flag in the gameboy's CPU
void clear_flag(uint8_t flag)
{
    cpu.f &= ~flag;
}


// Decrement the given 8-bit value and update the flags
void dec8(uint8_t *value)
{
    // Decrement the value
    *value -= 1;

    // Set the zero flag if the result is zero
    if (*value == 0)
    {
        set_flag(Z_FLAG);
    }
    else
    {
        clear_flag(Z_FLAG);
    }

    // Set the subtract flag
    set_flag(N_FLAG);

    // Set the half-carry flag if the lower 4 bits of the result are 0
    if ((*value & 0x0F) == 0)
    {
        set_flag(H_FLAG);
    }
    else
    {
        clear_flag(H_FLAG);
    }
}


// Decrement a 16-bit value
void dec16(uint16_t *value)
{
    // Decrement the value
    (*value)--;

    // Set the zero flag if the value is zero
    if (*value == 0)
    {
        set_flag(Z_FLAG);
    }

    // Set the half carry flag if the lower 4 bits of the value were 0 before the decrement
    if ((*value & 0x0F) == 0)
    {
        set_flag(H_FLAG);
    }

    // Clear the carry flag
    clear_flag(C_FLAG);

    // Set the negative flag
    set_flag(N_FLAG);
}


// Increment the given 8-bit value and update the flags
void inc(uint8_t *value)
{
    // Increment the value
    *value += 1;

    // Set the zero flag if the result is zero
    if (*value == 0)
    {
        set_flag(Z_FLAG);
    }
    else
    {
        clear_flag(Z_FLAG);
    }

    // Clear the subtract flag
    clear_flag(N_FLAG);

    // Set the half-carry flag if the lower 4 bits of the result are 0
    if ((*value & 0x0F) == 0)
    {
        set_flag(H_FLAG);
    }
    else
    {
        clear_flag(H_FLAG);
    }
}


// Increment an 8-bit value
void inc8(uint8_t *value)
{
    // Increment the value
    *value += 1;

    // Update the flags
    clear_flag(N_FLAG);
    set_flag_cond(Z_FLAG, *value == 0);
    clear_flag(H_FLAG);
    clear_flag(C_FLAG);
}

// Increment a 16-bit value
void inc16(uint16_t *value)
{
    // Increment the value
    *value += 1;

    // Update the flags
    clear_flag(N_FLAG);
    clear_flag(C_FLAG);
}


// Rotate the given 8-bit value left and update the flags
uint8_t rlc(uint8_t value)
{
    // Rotate the value left
    uint8_t carry = value & 0x80;
    value = (value << 1) | (carry >> 7);

    // Set the zero flag if the result is zero
    if (value == 0)
    {
        set_flag(Z_FLAG);
    }
    else
    {
        clear_flag(Z_FLAG);
    }

    // Clear the subtract flag
    clear_flag(N_FLAG);

    // Clear the half-carry flag
    clear_flag(H_FLAG);

    // Set the carry flag if a carry occurred
    if (carry != 0)
    {
        set_flag(C_FLAG);
    }
    else
    {
        clear_flag(C_FLAG);
    }

    // Return the rotated value
    return value;
}

uint8_t rrc(uint8_t value)
{
    uint8_t carry = value & 0x01;
    value = (value >> 1) | (carry << 7);
    if (value == 0)
    {
        // Set the zero flag
        cpu.f |= Z_FLAG;
    }
    else
    {
        // Clear the zero flag
        cpu.f &= ~Z_FLAG;
    }
    // Set the carry flag
    cpu.f |= (carry << 4);
    return value;
}


// Fetch a 16-bit word from the gameboy's memory
uint16_t fetch_word()
{
    // Fetch the lower 8 bits of the word
    uint8_t low = fetch_byte();

    // Fetch the upper 8 bits of the word
    uint8_t high = fetch_byte();

    // Return the 16-bit word
    return (high << 8) | low;
}


// Write a byte to the gameboy's memory
void write_byte(uint16_t address, uint8_t value)
{
    memory[address] = value;
}


// Add the values of the HL and BC registers
void add_hl(uint16_t* hl, uint16_t* bc)
{
    // Add the values of the HL and BC registers
    uint32_t result = *hl + *bc;

    // Set the HL register to the lower 16 bits of the result
    *hl = result & 0xFFFF;

    // Set the carry flag if there was a carry from bit 15 to bit 16
    if (result & 0x10000)
    {
        set_flag(C_FLAG);
    }
}

// Set a CPU flag if the given condition is true
void set_flag_cond(uint8_t flag, uint8_t cond)
{
    if (cond)
    {
        cpu.f |= flag;
    }
}
