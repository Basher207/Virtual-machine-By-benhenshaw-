#pragma once

// TODO: is this needed?
int wait_for_key;

#define u8 unsigned char
#define u16 unsigned short

#define MEMORY_LENGTH 4096
#define REGISTER_LENGTH 16
#define WIDTH 64
#define HEIGHT 32
#define STACK_LENGTH 16
#define KEY_LENGTH 16

#define OP_SIZE 2

#define PRESSED 1
#define RELEASED 0
#define BAD_KEY -1

int MapKey(int);

struct Chip8 {
    u8 memory[MEMORY_LENGTH];
    u16 v[REGISTER_LENGTH];
    u16 i;
    u16 pc;
    u8 screen[(WIDTH*HEIGHT)];
    u8 delay_timer;
    u8 sound_timer;
    u16 stack[STACK_LENGTH];
    u16 sp;
    u8 keys[KEY_LENGTH];
};

void Chip8_Init(struct Chip8 *);

void Chip8_Load(struct Chip8 *, FILE *);

void Chip8_Cycle(struct Chip8 *);

typedef void (* operation)(struct Chip8 *, u16);

void op_0(struct Chip8 * c, u16 op);
void op_1(struct Chip8 * c, u16 op);
void op_2(struct Chip8 * c, u16 op);
void op_3(struct Chip8 * c, u16 op);
void op_4(struct Chip8 * c, u16 op);
void op_5(struct Chip8 * c, u16 op);
void op_6(struct Chip8 * c, u16 op);
void op_7(struct Chip8 * c, u16 op);
void op_8(struct Chip8 * c, u16 op);
void op_9(struct Chip8 * c, u16 op);
void op_A(struct Chip8 * c, u16 op);
void op_B(struct Chip8 * c, u16 op);
void op_C(struct Chip8 * c, u16 op);
void op_D(struct Chip8 * c, u16 op);
void op_E(struct Chip8 * c, u16 op);
void op_F(struct Chip8 * c, u16 op);
