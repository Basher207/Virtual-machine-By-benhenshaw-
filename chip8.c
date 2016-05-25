#include <stdio.h>
#include <string.h>
#include "include.h"

operation ops[16] = {
    op_0, op_1, op_2, op_3, op_4, op_5, op_6, op_7,
    op_8, op_9, op_A, op_B, op_C, op_D, op_E, op_F
};

u8 chip8_fontset[80] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void Chip8_Init(struct Chip8 * c)
{
    // program begins at address 0x200
    c->pc = 0x200;
    c->i = 0;
    c->delay_timer = 0;
    c->sound_timer = 0;
    c->sp = 0;
    memset(c->memory, 0, MEMORY_LENGTH);
    memset(c->v, 0, REGISTER_LENGTH);
    memset(c->screen, 0, WIDTH*HEIGHT);
    memset(c->stack, 0, STACK_LENGTH);
    memset(c->keys, 0, KEY_LENGTH);
}

void Chip8_Load(struct Chip8 * c, FILE * f)
{
    for (int i = 0; i < 80; ++i) {
        c->memory[i] = chip8_fontset[i];
    }

    int data;
    int i = 0x200;
    while ((data = fgetc(f)) != EOF) {
        c->memory[i] = data;
        ++i;
    }
}

void Chip8_Cycle(struct Chip8 * c)
{
    u16 op = (c->memory[c->pc] << 8) | c->memory[c->pc + 1];
    printf("%04X: %04X\n", c->pc, op); // DEBUG

    ops[(op & 0xF000) >> 12](c, op);

    // NOTE: I don't think this should tick down every cycle
    if (c->delay_timer > 0) {
        c->delay_timer -= 1;
    }
    if (c->sound_timer > 0) {
        c->sound_timer -= 1;
    }
}
