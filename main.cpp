#include "chip8.h"
#include "chip8sdl.h"

#include <iostream>

int main(int argc, char* argv[]) {
    Chip8 chip8;
    std::string_view filename { argv[1] };
    
    chip8.loadRom(filename);

    // Set up SDL frontend
    Chip8sdl chip8sdl;
    chip8sdl.init();
   

    // Start emulation loop
    chip8sdl.run(chip8);

    return 0;
}

