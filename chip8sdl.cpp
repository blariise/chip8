#include "chip8sdl.h"
#include "chip8.h"

Chip8sdl::Chip8sdl()
  : window { nullptr }
  , renderer { nullptr }
  , texture { nullptr } {}

Chip8sdl::~Chip8sdl() {}

bool Chip8sdl::init() {

  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    return false;

  window = SDL_CreateWindow(
      "chip8",
      640,
      320,
      SDL_WINDOW_OPENGL);
  if (!window)
    return false;

  renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer)
    return false;

  return true;
}

void Chip8sdl::run(Chip8& chip8) {
  while (true) {
    handleInput(chip8);
    chip8.cycle();
    updateDisplay(chip8);
    SDL_Delay(2);
  }
}

void Chip8sdl::handleInput(Chip8& chip8) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      
      case SDL_EVENT_QUIT:
        exit(0);
        break;

      case SDL_EVENT_KEY_DOWN:
      case SDL_EVENT_KEY_UP:
        bool pressed (event.type == SDL_EVENT_KEY_DOWN);
        SDL_Keycode keycode { event.key.key };

        for (std::size_t i { 0 }; i < std::size(keymap); ++i) {
          if ( keycode == keymap[i]) {
            chip8.setKeyState(i, pressed);
            break;
          }
        }
    }
  }
}

void Chip8sdl::updateDisplay(const Chip8& chip8) {
  auto& display { chip8.getDisplay() };
  constexpr int scale { 10 };

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // clear display
  SDL_RenderClear(renderer);
  
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  for (int y { 0 }; y < 32; ++y) {
    for (int x { 0 }; x < 64; ++x) {
      if (display[y * 64 + x]) {
        SDL_FRect pixel {
          static_cast<float>(x * scale),
          static_cast<float>(y * scale, scale) };
        SDL_RenderFillRect(renderer, &pixel);
      }
    }
  }
  SDL_RenderPresent(renderer);
}

