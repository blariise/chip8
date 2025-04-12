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

  private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    std::array<SDL_Keycode, 16> keymap {
      SDLK_1, SDLK_2, SDLK_3, SDLK_4,
      SDLK_Q, SDLK_W, SDLK_E, SDLK_R,
      SDLK_A, SDLK_S, SDLK_D, SDLK_F,
      SDLK_Z, SDLK_X, SDLK_C, SDLK_V,
    };
};
#endif // CHIP8SDL_H

