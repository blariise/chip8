#pragma once

class SDL_Window;
class SDL_Renderer;
class SDL_Texture;

class Render {
    private:
        SDL_Window *window{};
        SDL_Renderer *renderer{};
        SDL_Texture *texture{};
    
    public:
        Render(char const *title, int renderWidth, int renderHeight, int textureWidth, int textureHeight);
        ~Render();
        void update(void const *buffer, int pitch);
        bool processInput(unsigned char *keys);
}