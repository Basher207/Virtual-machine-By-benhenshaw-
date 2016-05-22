#Compiler to be used:
CC=clang

SDL_LINK = -framework SDL2

project_Main = main.c
Chip8_reader = chip8.c
ops_instruction_set = ops.c

OutputName = VM

all:
	make SDL_Handler


SDL_Handler:
	$(CC) $(project_Main) $(Chip8_reader) $(ops_instruction_set) $(SDL_LINK) -o $(OutputName).o