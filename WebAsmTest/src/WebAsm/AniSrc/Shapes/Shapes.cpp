#include "Shapes.h"

void DrawPixel(SDL_Renderer* renderer, int x, int y, Uint8 r, Uint8 g, Uint8 b,
               Uint8 a, int size) {
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_Rect rect = {x, y, size, size};
  SDL_RenderFillRect(renderer, &rect);
}