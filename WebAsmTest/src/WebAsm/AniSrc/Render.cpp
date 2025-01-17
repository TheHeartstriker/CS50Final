#include "Render.h"

#include <random>
#include <string>
#include <vector>

#include "Render.h"
#include "Shapes/Shapes.h"

extern int Winwidth;
extern int Winheight;

int intialWinwidth = Winwidth;
int intialWinheight = Winheight;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(0, Winwidth);
std::uniform_real_distribution<float> dis_Num(1.0, 5.0);
// False means go down, true means go up
struct Pixel {
  int x;
  float y;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  float speed;
  bool Direction = true;
};
// Stores pixels data
static std::vector<Pixel> pixels;
// Creates a hundred instances of the Pixel struct
void initPixels(std::vector<Pixel>& pixels) {
  if (Winheight) {
    for (int i = 0; i < 2000; i++) {
      if (i % 2 == 0) {
        pixels.push_back({dis(gen), 5.0f, 15, 220, 30, dis_Num(gen), false});
      } else {
        pixels.push_back({dis(gen), static_cast<float>(Winheight - 5), 15, 220,
                          30, dis_Num(gen), true});
      }
    }
  }
}

void MainCall(SDL_Renderer* renderer) {
  // Initialize the pixels
  if (pixels.empty()) {
    initPixels(pixels);
  }
  if (Winwidth != intialWinwidth || Winheight != intialWinheight) {
    pixels.clear();
    intialWinwidth = Winwidth;
    intialWinheight = Winheight;
    dis = std::uniform_int_distribution<int>(0, Winwidth);
    initPixels(pixels);
  }
  // Draw the pixels
  for (auto& pixel : pixels) {
    // If at center
    if (pixel.y > Winheight / 2 - 10 && pixel.y < Winheight / 2 + 10) {
      pixel.x = dis(gen);
      if (pixel.Direction) {
        pixel.y = static_cast<float>(Winheight - 5);
        pixel.speed = dis_Num(gen);
      } else {
        pixel.y = 0.0f;
        pixel.speed = dis_Num(gen);
      }
      continue;
      // Up vs down fall logic
    } else if (pixel.Direction) {
      pixel.y -= pixel.speed;
      pixel.speed -= 0.1;
      DrawPixel(renderer, pixel.x, pixel.y, pixel.r, pixel.g, pixel.b, 0, 4);
    } else {
      pixel.speed -= 0.1;
      pixel.y += pixel.speed;
      DrawPixel(renderer, pixel.x, pixel.y, pixel.r, pixel.g, pixel.b, 0, 4);
    }
  }
}
