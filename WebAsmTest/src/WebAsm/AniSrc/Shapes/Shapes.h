#ifndef SHAPES_H
#define SHAPES_H

#include <SDL2/SDL.h>

void DrawPixel(SDL_Renderer* renderer, int x, int y, Uint8 r, Uint8 g, Uint8 b,
               Uint8 a, int size = 1);
void ChoasOrb(SDL_Renderer* renderer, int radius, int cx, int cy);
#endif