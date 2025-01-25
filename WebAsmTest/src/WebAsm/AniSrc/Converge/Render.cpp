#include "Render.h"

#include <random>
#include <string>
#include <vector>

#include "../Shapes/Shapes.h"
#include "Render.h"

extern int Winwidth;
extern int Winheight;

int intialWinwidth = Winwidth;
int intialWinheight = Winheight;

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<int> dis(0, Winwidth);
static std::uniform_real_distribution<float> dis_Num(0.3, 0.9);
static std::uniform_int_distribution<Uint8> dis_Color(0, 255);

// Name space to free up global space
namespace {
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
}  // namespace
// Stores pixels data
static std::vector<Pixel> pixels;
// Creates a hundred instances of the Pixel struct
static void initPixels(std::vector<Pixel>& pixels) {
  if (Winheight) {
    for (int i = 0; i < 50000; i++) {
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
static void WindowSizeChange() {
  if (Winwidth != intialWinwidth || Winheight != intialWinheight) {
    pixels.clear();
    intialWinwidth = Winwidth;
    intialWinheight = Winheight;
    dis = std::uniform_int_distribution<int>(0, Winwidth);
    initPixels(pixels);
  }
}
// Out of bounds? General reset
static void ResetPixel(Pixel& Pix) {
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

void MainConvergeCall(SDL_Renderer* renderer) {
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
    // Chance to disappear if close to center
    if (pixel.y > Winheight / 2 - 65 && pixel.y < Winheight / 2 + 65 &&
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
      DrawPixel(renderer, pixel.x, pixel.y, pixel.r, pixel.g, pixel.b, 0, 5);
    } else {
      pixel.y += pixel.speed / slowFactor;
      DrawPixel(renderer, pixel.x, pixel.y, pixel.r, pixel.g, pixel.b, 0, 5);
    }
  }
}
