#include <SDL2/SDL.h>
#include <emscripten.h>

#include <cmath>
#include <iostream>

#include "AniSrc/Render.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

int Winheight;
int Winwidth;

void render();

extern "C" {
EMSCRIPTEN_KEEPALIVE
void setArguments(int arg1, int arg2) {
  Winheight = arg1;
  Winwidth = arg2;

  // Update the window size
  SDL_SetWindowSize(window, Winwidth, Winheight);
}
}

void render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  MainCall(renderer);

  SDL_RenderPresent(renderer);
}

int main() {
  // Intialize SDL
  SDL_Init(SDL_INIT_VIDEO);
  // Create a window and renderer
  window = SDL_CreateWindow("WASM SDL", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, Winheight, Winwidth,
                            SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  // Main loop
  emscripten_set_main_loop(render, 0, 1);
  // Exit code
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}