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
std::uniform_real_distribution<float> dis_Num(0.3, 0.9);
std::uniform_int_distribution<Uint8> dis_Color(0, 255);

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
    for (int i = 0; i < 60000; i++) {
      if (i % 2 == 0) {
        pixels.push_back({dis(gen), 2.0f, dis_Color(gen), dis_Color(gen),
                          dis_Color(gen), dis_Num(gen), false});
      } else {
        pixels.push_back({dis(gen), static_cast<float>(Winheight - 2),
                          dis_Color(gen), dis_Color(gen), dis_Color(gen),
                          dis_Num(gen), true});
      }
    }
  }
}
// Instructions for window size change
void WindowSizeChange() {
  if (Winwidth != intialWinwidth || Winheight != intialWinheight) {
    pixels.clear();
    intialWinwidth = Winwidth;
    intialWinheight = Winheight;
    dis = std::uniform_int_distribution<int>(0, Winwidth);
    initPixels(pixels);
  }
}

void ResetPixel(Pixel& Pix) {
  Pix.x = dis(gen);
  Pix.speed = dis_Num(gen);
  Uint8 newR = dis_Color(gen);
  Uint8 newG = dis_Color(gen);
  Uint8 newB = dis_Color(gen);

  if (Pix.Direction) {
    Pix.y = static_cast<float>(Winheight);
  } else {
    Pix.y = 0.0f;
  }

  Pix.r = newR;
  Pix.g = newG;
  Pix.b = newB;
}

void MainCall(SDL_Renderer* renderer) {
  static const float Decay = 3.0f;
  // Initialize the pixels
  if (pixels.empty()) {
    initPixels(pixels);
  }
  // If window size changes
  WindowSizeChange();
  // Draw the pixels
  for (auto& pixel : pixels) {
    float distanceFromCenter = std::abs(pixel.y - Winheight / 2);
    float slowFactor = std::exp(-distanceFromCenter / (Winheight / 2) * Decay);

    if (pixel.y > Winheight / 2 - 45 && pixel.y < Winheight / 2 + 45 &&
        dis_Color(gen) < 10) {
      ResetPixel(pixel);
      continue;
    }

    // If at center
    if (pixel.y > Winheight / 2 - 12 && pixel.y < Winheight / 2 + 12) {
      ResetPixel(pixel);
      continue;
    }
    // Color change
    pixel.r =
        static_cast<Uint8>(std::min(255.0f, pixel.r + 0.05f / slowFactor));
    pixel.g = static_cast<Uint8>(std::max(0.0f, pixel.g - 0.03f / slowFactor));
    pixel.b = static_cast<Uint8>(std::max(0.0f, pixel.b - 0.03f / slowFactor));
    // Up vs down fall logic
    if (pixel.Direction) {
      pixel.y -= pixel.speed / slowFactor;
      DrawPixel(renderer, pixel.x, pixel.y, pixel.r, pixel.g, pixel.b, 0, 2);
    } else {
      pixel.y += pixel.speed / slowFactor;
      DrawPixel(renderer, pixel.x, pixel.y, pixel.r, pixel.g, pixel.b, 0, 2);
    }
  }
}
