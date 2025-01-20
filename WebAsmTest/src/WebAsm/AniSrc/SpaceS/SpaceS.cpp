#include "SpaceS.h"

#include <array>
#include <random>
#include <vector>

#include "../Shapes/Shapes.h"

extern int Winwidth;
extern int Winheight;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(0, Winwidth);
std::uniform_int_distribution<Uint8> dis_Color(0, 255);
std::uniform_int_distribution<Uint8> direction(10, 20);

struct Pixel {
  // Location
  Uint16 x;
  Uint16 y;
  // Color
  Uint8 r;
  Uint8 g;
  Uint8 b;
  float speed;
  std::array<Uint16, 2> Direction;
};

std::vector<int> CirlceIds;

static std::vector<Pixel> pixels;

void initPixels(std::vector<Pixel>& pixels, int radius, int cx, int cy) {
  if (Winheight) {
    // Random valid x and y range
    std::uniform_real_distribution<float> angle_dist(0.0f, 2.0f * M_PI);
    std::uniform_real_distribution<float> radius_dist(
        0.0f, static_cast<float>(radius));

    for (int i = 0; i < 50; i++) {
      float angle = angle_dist(gen);
      float r = radius_dist(gen);
      Uint16 x = static_cast<Uint16>(cx + r * cos(angle));
      Uint16 y = static_cast<Uint16>(cy + r * sin(angle));

      pixels.push_back({x,
                        y,
                        dis_Color(gen),
                        dis_Color(gen),
                        dis_Color(gen),
                        2.0f,
                        {
                            static_cast<Uint16>(direction(gen)),
                            static_cast<Uint16>(direction(gen)),
                        }});
    }
  }
}
void MainSpacesCall(SDL_Renderer* renderer) {
  if (pixels.empty()) {
    initPixels(pixels, 100, 100, 100);
  }
  for (auto& pixel : pixels) {
    // Temp distance of 100
    float dx = static_cast<float>(100 - pixel.x);
    float dy = static_cast<float>(100 - pixel.y);
    float distance = sqrt(dx * dx + dy * dy);
    if (distance <= 90) {
      pixel.x += pixel.Direction[0] / 2;
      pixel.y += pixel.Direction[1] / 2;
    }
    // else if (distance > 110) {
    //   // Random valid x and y range
    //   std::uniform_real_distribution<float> angle_dist(0.0f, 2.0f * M_PI);
    //   std::uniform_real_distribution<float> radius_dist(
    //       0.0f, static_cast<float>(100));

    //   float angle = angle_dist(gen);
    //   float r = radius_dist(gen);
    //   Uint16 x = static_cast<Uint16>(100 + r * cos(angle));
    //   Uint16 y = static_cast<Uint16>(100 + r * sin(angle));
    //   pixel.x = x;
    //   pixel.y = y;
    else {
      pixel.Direction[0] = pixel.Direction[0] * -1;
      pixel.Direction[1] = pixel.Direction[1] * -1;
      pixel.x += pixel.Direction[0] / 2;
      pixel.y += pixel.Direction[1] / 2;
    }

    DrawPixel(renderer, pixel.x, pixel.y, pixel.r, pixel.g, pixel.b, 0, 3);
  }
  ChoasOrb(renderer, 100, 100, 100);
}