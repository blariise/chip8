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

  texture = SDL_CreateTexture(
      renderer,
      SDL_PIXELFORMAT_RGBA8888,
      SDL_TEXTUREACCESS_STREAMING,
      64, 32);
  if (!texture)
    return false;

  return true;
}


int main() {
  return 0;
}
