#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "include.h"

// TODO: make screen 1 bit per pixel; sprites are stored this way
//       so drawing would be copying byte by byte to buffer.

int main(int argc, char * argv[])
{
    // DEBUG:
    setvbuf(stdout, NULL, _IONBF, 0);

    if (argc < 2) {
        fprintf(stderr, "chip8: No program specified.\nUSAGE:\n\tchip8 'program'\n");
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("ERROR: %s\n", SDL_GetError());
        return 1;
    }

    // TODO: get and validate from args
    const char * file_name = argv[1];
    int scale = 8;

    // TODO: handle errors
    SDL_Window* window = SDL_CreateWindow("Chip-8",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH*scale, HEIGHT*scale, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // TODO: clean up!
    wait_for_key = 0;

    srand(time(NULL));

    struct Chip8 chip8;
    Chip8_Init(&chip8);

    FILE * f = fopen(file_name, "rb");
    if (f) {
        Chip8_Load(&chip8, f);
        fclose(f);
    } else {
        printf("Could not open file: '%s'\n", file_name);
        return 1;
    }

    int running = 1;
    while (running) {
        SDL_Event event;
        // TODO: replace wait_for_key; very high cpu use, delays quitting
        while (SDL_PollEvent(&event) || wait_for_key) {
            switch (event.type) {
                case SDL_QUIT: {
                    wait_for_key = 0;
                    running = 0;
                    break;
                } break;
                case SDL_KEYDOWN: {
                    Chip8_Cycle(&chip8);
                    if (!event.key.repeat) {
                        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                            running = 0;
                            break;
                        }
                        int key;
                        if ((key = MapKey(event.key.keysym.scancode)) != BAD_KEY) {
                            wait_for_key = 0;
                            chip8.keys[key] = 1;
                        }
                    }
                } break;
                case SDL_KEYUP: {
                    int key;
                    if ((key = MapKey(event.key.keysym.scancode)) != BAD_KEY) {
                        chip8.keys[key] = 0;
                    }
                } break;
            }

            // TODO: remove
            if (wait_for_key) {
                SDL_Delay(8);
            }
        }

        Chip8_Cycle(&chip8);

        // TODO: audio
        // if (chip8.sound_timer == 0) {
        //     beep();
        // }

        SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 242, 240, 236, 255);
        for (int x = 0; x < WIDTH; ++x) {
            for (int y = 0; y < HEIGHT; ++y) {
                if (chip8.screen[x + y * WIDTH]) {
                    const struct SDL_Rect rect = { x * scale, y * scale, scale, scale };
                    // TODO: minimise draw calls
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }

        SDL_RenderPresent(renderer);

        // TODO: Accurate timing; CPU should operate at 1KHz
        SDL_Delay(2);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

int MapKey(int scancode) {
    switch (scancode) {
        case SDL_SCANCODE_1: {
            return 0;
        } break;
        case SDL_SCANCODE_2: {
            return 1;
        } break;
        case SDL_SCANCODE_3: {
            return 2;
        } break;
        case SDL_SCANCODE_4: {
            return 3;
        } break;
        case SDL_SCANCODE_Q: {
            return 4;
        } break;
        case SDL_SCANCODE_W: {
            return 5;
        } break;
        case SDL_SCANCODE_E: {
            return 6;
        } break;
        case SDL_SCANCODE_R: {
            return 7;
        } break;
        case SDL_SCANCODE_A: {
            return 8;
        } break;
        case SDL_SCANCODE_S: {
            return 9;
        } break;
        case SDL_SCANCODE_D: {
            return 10;
        } break;
        case SDL_SCANCODE_F: {
            return 11;
        } break;
        case SDL_SCANCODE_Z: {
            return 12;
        } break;
        case SDL_SCANCODE_X: {
            return 13;
        } break;
        case SDL_SCANCODE_C: {
            return 14;
        } break;
        case SDL_SCANCODE_V: {
            return 15;
        } break;
        default: {
            return BAD_KEY;
        }
        break;
    }
}
