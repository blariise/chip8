#ifndef CHIP8SDL_H
#define CHIP8SDL_H

#include <iostream>
#include <memory>
#include "chip8.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>

class Chip8sdl {
  public:
    Chip8sdl();
    ~Chip8sdl();

    bool init();
    void run(Chip8& chip8);
    void updateDisplay(const Chip8& chip8);
    void handleInput(Chip8& chip8);
    void debugDisplay(const Chip8& chip8);

  private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    // idk how i mapped this
    // 1 2 3 C
    // 4 5 6 D
    // 7 8 9 E
    // A 0 B F
    std::array<SDL_Keycode, 16> keymap {
      SDLK_X, SDLK_1, SDLK_2, SDLK_3,
      SDLK_Q, SDLK_W, SDLK_E, SDLK_A,
      SDLK_S, SDLK_D, SDLK_Z, SDLK_C,
      SDLK_4, SDLK_R, SDLK_F, SDLK_V,
    };
};
#endif // CHIP8SDL_H

