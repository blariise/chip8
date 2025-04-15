#include "chip8sdl.h"
#include "chip8.h"

Chip8sdl::Chip8sdl()
  : window { nullptr }
  , renderer { nullptr } {}

Chip8sdl::~Chip8sdl() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

bool Chip8sdl::init() {

  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    return false;

  window = SDL_CreateWindow(
      "chip8",
      640,
      320,
      SDL_WINDOW_RESIZABLE);
  if (!window)
    return false;

  renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer)
    return false;

  return true;
}

void Chip8sdl::run(Chip8& chip8) {
  const int TIMER_INTERVAL = 1000 / 60; // 60Hz
  Uint64 lastTimer { SDL_GetTicks() };

  while (true) {
    handleInput(chip8);
    chip8.cycle();
    updateDisplay(chip8);

    Uint64 now { SDL_GetTicks() };
    if (now - lastTimer >= TIMER_INTERVAL) {
      chip8.updateTimers();
      lastTimer = now;
    }

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
          static_cast<float>(y * scale),
          static_cast<float>(scale),
          static_cast<float>(scale)};
        SDL_RenderFillRect(renderer, &pixel);
      }
    }
  }
  SDL_RenderPresent(renderer);
}

void Chip8sdl::debugDisplay(const Chip8& chip8) {
  auto& display = chip8.getDisplay();
  for (int y = 0; y < 32; y++) {
    for (int x = 0; x < 64; x++) {
      std::cout << (display[y * 64 + x] ? "█" : " ");
    }
    std::cout << "\n";
  }
}
