package main

import (
	sdl "github.com/veandco/go-sdl2/sdl"
	//chip8 "github.com/blariise/chip8/Chip-8"
	//"fmt"
)

const (
	chipWidth  int32 = 64
	chipHeight int32 = 32
)

func main() {

	initSDL() // initialize SDL2

	// Create window
	window, err := sdl.CreateWindow("test", sdl.WINDOWPOS_UNDEFINED, sdl.WINDOWPOS_UNDEFINED,
		chipWidth*20, chipHeight*20, sdl.WINDOW_SHOWN)
	defer window.Destroy()

	if err != nil {
		panic(err)
	}

	// loop of app
	running := true
	for running {
		for event := sdl.PollEvent(); event != nil; event = sdl.PollEvent() {
			switch event.(type) {
			case *sdl.QuitEvent:
				println("Quit")
				running = false
			}
		}
	}
}

func initSDL() {
	if err := sdl.Init(sdl.INIT_EVERYTHING); err != nil {
		panic(err)
	}
	defer sdl.Quit()
}
