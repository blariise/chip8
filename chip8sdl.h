#ifndef CHIP8SDL_H
#define CHIP8SDL_H

#include <iostream>
#include <memory>
#include <chip8.h>

#include <SDL3/SDL.h>

class Chip8sdl {
  public:
    Chip8sdl();
    ~Chip8sdl();

    void init();
    void run(Chip8& chip8);
    void updateDisplay(const Chip8& chip8);
    void handleInput(Chip8& chip8);

  private:
    std::unique_ptr<SDL_Window> window;
    std::unique_ptr<SDL_Renderer> renderer;
    std::unique_ptr<SDL_Texture> texture;

    std::array<SDL_Keycode, 16> keymap {
      SDLK_1, SDLK_2, SDLK_3, SDLK_4,
      SDLK_q, SDLK_w, SDLK_e, SDLK_r,
      SDLK_a, SDLK_s, SDLK_d, SDLK_f,
      SDLK_z, SDLK_x, SDLK_c, SDLK_v,
    };
};
#endif // CHIP8SDL_H

