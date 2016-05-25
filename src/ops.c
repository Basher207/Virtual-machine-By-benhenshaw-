#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include.h"

void op_0(struct Chip8 * c, u16 op)
{
    switch (op & 0x00FF) {
        case 0x00E0: {
            // 00E0: Clear the screen
            memset(c->screen, 0, WIDTH*HEIGHT);
        } break;
        case 0x00EE: {
            // 00EE: Return from a subroutine
            c->sp -= 1;
            c->pc = c->stack[c->sp];
        } break;
    }
    c->pc += OP_SIZE;
}

void op_1(struct Chip8 * c, u16 op)
{
    // 1NNN: Jump to address NNN
    c->pc = op & 0x0FFF;
}

void op_2(struct Chip8 * c, u16 op)
{
    // 2NNN: Execute subroutine starting at address NNN
    c->stack[c->sp] = c->pc;
    c->sp += 1;
    c->pc = op & 0x0FFF;
}

void op_3(struct Chip8 * c, u16 op)
{
    // 3XNN: Skip the following instruction if the value
    //       of register VX equals NN
    u8 x = (op & 0x0F00) >> 8;
    u8 n = op & 0x00FF;
    if (c->v[x] == n) {
        c->pc += OP_SIZE * 2;
    } else {
        c->pc += OP_SIZE;
    }
}

void op_4(struct Chip8 * c, u16 op)
{
    // 4XNN: Skip the following instruction if the value
    //       of register VX is not equal to NN
    u8 x = (op & 0x0F00) >> 8;
    u8 n = op & 0x00FF;
    if (c->v[x] != n) {
        c->pc += OP_SIZE * 2;
    } else {
        c->pc += OP_SIZE;
    }
}

void op_5(struct Chip8 * c, u16 op)
{
    // 5XY0: Skip the following instruction if the value
    //       of register VX is equal to the value of
    //       register VY
    u8 x = (op & 0x0F00) >> 8;
    u8 y = (op & 0x00F0) >> 4;
    if (c->v[x] == c->v[y]) {
        c->pc += OP_SIZE * 2;
    } else {
        c->pc += OP_SIZE;
    }
}

void op_6(struct Chip8 * c, u16 op)
{
    // 6XNN: Store number NN in register VX
    c->v[(op & 0x0F00) >> 8] = op & 0x00FF;
    c->pc += OP_SIZE;
}

void op_7(struct Chip8 * c, u16 op)
{
    // 7XNN: Add the value NN to register VX
    c->v[(op & 0x0F00) >> 8] += op & 0x00FF;
    c->pc += OP_SIZE;
}

void op_8(struct Chip8 * c, u16 op)
{
    u8 x = (op & 0x0F00) >> 8;
    u8 y = (op & 0x00F0) >> 4;

    switch (op & 0x000F) {
        case 0x0000: {
            // 8XY0: Store the value of register VY in register VX
            c->v[x] = c->v[y];
        } break;
        case 0x0001: {
            // 8XY1: Set VX to VX OR VY
            c->v[x] |= c->v[y];
        } break;
        case 0x0002: {
            // 8XY2: Set VX to VX AND VY
            c->v[x] &= c->v[y];
        } break;
        case 0x0003: {
            // 8XY3: Set VX to VX XOR VY
            c->v[x] ^= c->v[y];
        } break;
        case 0x0004: {
            // 8XY4: Add the value of register VY to register VX
            //       Set VF to 01 if a carry occurs Set VF to 00
            //       if a carry does not occur
            c->v[0xF] = (c->v[x] + c->v[y]) > 0xFFFF;
            c->v[x] += c->v[y];
        } break;
        case 0x0005: {
            // 8XY5: Subtract the value of register VY from
            //       register VX; Set VF to 00 if a borrow occurs
            //       Set VF to 01 if a borrow does not occur
            c->v[0xF] = (c->v[x] - c->v[y]) > 0;
            c->v[x] -= c->v[y];
        } break;
        case 0x0006: {
            // 8XY6: Shift VX right by one bit; Set register VF to
            //       the least significant bit before the shift
            c->v[0xF] = c->v[x] & 1;
            c->v[x] >>= 1;
        } break;
        case 0x0007: {
            // 8XY7: Set register VX to the value of VY minus VX
            //       Set VF to 00 if a borrow occurs; Set VF to 01
            //       if a borrow does not occur
            c->v[0xF] = (c->v[y] - c->v[x]) < 0;
            c->v[x] = c->v[y] - c->v[x];
        } break;
        case 0x000E: {
            // 8XYE: Shift VX left by one bit; Set register VF to
            //       the most significant bit before the shift
            c->v[0xF] = c->v[x] >> 7;
            c->v[x] <<= 1;
        } break;
    }
    c->pc += OP_SIZE;
}

void op_9(struct Chip8 * c, u16 op)
{
    // 9XY0: Skip the following instruction if the value
    //       of register VX is not equal to the value of
    //       register VY
    u8 x = (op & 0x0F00) >> 8;
    u8 y = (op & 0x00F0) >> 4;
    if (c->v[x] != c->v[y]) {
        c->pc += OP_SIZE * 2;
    } else {
        c->pc += OP_SIZE;
    }
}

void op_A(struct Chip8 * c, u16 op)
{
    // ANNN: Store memory address NNN in register I
    c->i = op & 0x0FFF;
    c->pc += OP_SIZE;
}

void op_B(struct Chip8 * c, u16 op)
{
    // BNNN: Jump to address NNN + V0
    c->pc = (op & 0x0FFF) + c->v[0];
}

void op_C(struct Chip8 * c, u16 op)
{
    // CXNN: Set VX to a random number with a mask of NN
    c->v[(op & 0x0F00) >> 8] = rand() & (op & 0x00FF);
    c->pc += OP_SIZE;
}

void op_D(struct Chip8 * c, u16 op)
{
    // DXYN: Draw a sprite at position VX, VY with N bytes
    //       of sprite data starting at the address stored
    //       in I; Set VF to 01 if any set pixels are
    //       changed to unset, and 00 otherwise
    int xpos = c->v[(op & 0x0F00) >> 8];
    int ypos = c->v[(op & 0x00F0) >> 4];
    int rows = (op & 0x000F);

    for (int row = 0; row < rows; ++row) {
        u8 pixel = c->memory[c->i + row];
        for (int col = 0; col < 8; ++col) {
            if (pixel & (128 >> col)) {
                u16 i = (xpos + col + (ypos + row) * WIDTH) % (WIDTH*HEIGHT);
                c->v[0xF] = c->screen[i];
                c->screen[i] ^= 1;
            }
        }
    }

    c->pc += OP_SIZE;
    // TODO: is draw flag needed?
}

void op_E(struct Chip8 * c, u16 op)
{
    u8 x = (op & 0x0F00) >> 8;
    printf("KEY: %X = %d\n", c->v[x], c->keys[c->v[x]]);

    switch (op & 0x00FF) {
        case 0x009E: {
            // EX9E: Skip the following instruction if the key
            //       corresponding to the hex value currently
            //       stored in register VX is pressed
            if (c->keys[c->v[x]]) {
                c->pc += OP_SIZE * 2;
            } else {
                c->pc += OP_SIZE;
            }
        } break;
        case 0x00A1: {
            // EXA1: Skip the following instruction if the key
            //       corresponding to the hex value currently
            //       stored in register VX is not pressed
            if (c->keys[c->v[x]]) {
                c->pc += OP_SIZE;
            } else {
                c->pc += OP_SIZE * 2;
            }
        } break;
    }
    c->pc += OP_SIZE;
}

void op_F(struct Chip8 * c, u16 op)
{
    u8 x = (op & 0x0F00) >> 8;

    switch (op & 0x00FF) {
        case 0x0007: {
            // FX07: Store the current value of the delay timer in
            //       register VX
            c->v[x] = c->delay_timer;
        } break;
        case 0x000A: {
            // FX0A: Wait for a keypress and store the result in
            //       register VX
            // TODO: store key in VX
            wait_for_key = 1;
        } break;
        case 0x0015: {
            // FX15: Set delay timer to the value of register VX
            c->delay_timer = c->v[x];
        } break;
        case 0x0018: {
            // FX18: Set sound timer to the value of register VX
            c->sound_timer = c->v[x];
        } break;
        case 0x001E: {
            // FX1E: Add the value in register VX to register I
            c->v[0xF] = c->i + c->v[x] > 0xFFF;
            c->i += c->v[x];
        } break;
        case 0x0029: {
            // FX29: Set I to the memory address of the sprite data
            //       corresponding to the hexadecimal digit stored
            //       in register VX
            // font is 5 bytes per character, starting at address 0
            c->i = c->v[x] * 5;
        } break;
        case 0x0033: {
            // FX33: Store the binary-coded decimal equivalent of
            //       the value stored in register VX at addresses
            //       I, I+1, and I+2
            c->memory[c->i]     = c->v[x] / 100;
            c->memory[c->i + 1] = (c->v[x] / 10) % 10;
            c->memory[c->i + 2] = (c->v[x] % 100) % 10;
        } break;
        case 0x0055: {
            // FX55: Store the values of registers V0 to VX in
            //       memory starting at address I; I is set to
            //       I + X + 1 after operation
            for (int i = 0; i <= x; ++i) {
                c->memory[c->i + i] = c->v[i];
            }
            c->i += x + 1;
        } break;
        case 0x0065: {
            // FX65: Fill registers V0 to VX inclusive with the
            //       values in memory starting at address I;
            //       I is set to I + X + 1 after operation
            for (int i = 0; i <= x; ++i) {
                c->v[i] = c->memory[c->i + i];
            }
            c->i += x + 1;
        } break;
    }
    c->pc += OP_SIZE;
}
