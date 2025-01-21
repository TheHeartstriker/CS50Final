#include "SpaceS.h"

#include <array>
#include <random>
#include <vector>

#include "../Shapes/Shapes.h"

extern int Winwidth;
extern int Winheight;
const int radius = 100;
int LocatinX = Winwidth / 2;
int LocatinY = Winheight / 2;
int intialWidth = Winwidth;
int intialHeight = Winheight;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(0, Winwidth);
std::uniform_int_distribution<Uint8> dis_Color(0, 255);
std::uniform_real_distribution<float> speed_dis(-3, 3);

float NoZero() {
  int num = speed_dis(gen);
  while (num == 0) {
    num = speed_dis(gen);
  }
  return num;
}

struct Pixel {
  // Location
  int x;
  int y;
  // Color
  Uint8 r;
  Uint8 g;
  Uint8 b;
  // Speed val
  float speedX;
  float speedY;
};

struct Point {
  int x;
  int y;
};

std::vector<int> CirlceIds;

static std::vector<Pixel> pixels;

void initPixels(std::vector<Pixel>& pixels, int radius, int cx, int cy) {
  if (Winheight) {
    // Random valid x and y range
    std::uniform_real_distribution<float> angle_dist(0.0f, 2.0f * M_PI);
    std::uniform_real_distribution<float> radius_dist(
        0.0f, static_cast<float>(radius));

    for (int i = 0; i < 5000; i++) {
      float angle = angle_dist(gen);
      float r = radius_dist(gen);
      int x = (cx + r * cos(angle));
      int y = (cy + r * sin(angle));

      pixels.push_back({
          x,
          y,
          dis_Color(gen),
          dis_Color(gen),
          dis_Color(gen),
          NoZero(),
          NoZero(),
      });
    }
  }
}

std::vector<Point> generateCircularPath(int centerX, int centerY, int radius,
                                        int numPoints) {
  std::vector<Point> path;
  float angleIncrement = 2.0f * M_PI / numPoints;

  for (int i = 0; i < numPoints; ++i) {
    float angle = i * angleIncrement;
    int x = centerX + static_cast<int>(radius * cos(angle));
    int y = centerY + static_cast<int>(radius * sin(angle));
    path.push_back({x, y});
  }

  return path;
}

void Around(SDL_Renderer* renderer) {
  Pixel AroundPix;
  std::vector<Point> path =
      generateCircularPath(LocatinX, LocatinY, radius * 2, 36);
  AroundPix.x = path[0].x;
  AroundPix.y = path[0].y;
  AroundPix.r = 255;  // Example initialization
  AroundPix.g = 255;  // Example initialization
  AroundPix.b = 255;  // Example initialization
  for (int i = 1; i < path.size(); i++) {
    AroundPix.x = path[i].x;
    AroundPix.y = path[i].y;
    DrawPixel(renderer, AroundPix.x, AroundPix.y, AroundPix.r, AroundPix.g,
              AroundPix.b, 0, 5);
  }
}

// Move a point closer to another point
void MoveCloser(int& x2, int& y2, int x, int y, int units) {
  // Calculate the difference
  int dx = (x2)-x;
  int dy = (y2)-y;

  // Calculate the distance
  float distance = sqrt(dx * dx + dy * dy);
  // Normalize
  if (distance != 0) {
    dx = (dx / distance * units);
    dy = (dy / distance * units);
  }
  // Move the point closer
  x2 = ((x2)-dx);
  y2 = ((y2)-dy);
}

void CheckWindowChange() {
  if (Winwidth != intialWidth || Winheight != intialHeight) {
    pixels.clear();
    intialWidth = Winwidth;
    intialHeight = Winheight;
    LocatinX = Winwidth / 2;
    LocatinY = Winheight / 2;
    initPixels(pixels, radius, LocatinX, LocatinY);
  }
}

void MainSpacesCall(SDL_Renderer* renderer) {
  if (pixels.empty()) {
    initPixels(pixels, radius, LocatinX, LocatinY);
  }
  CheckWindowChange();
  for (auto& pixel : pixels) {
    // Temp distance of 100
    int Distance =
        sqrt(pow(pixel.x - LocatinX, 2) + pow(pixel.y - LocatinY, 2));
    if (Distance > radius) {
      MoveCloser(pixel.x, pixel.y, LocatinX, LocatinY, 10);
      // Then reverse the direction
      pixel.speedX = -pixel.speedX;
      pixel.speedY = -pixel.speedY;
    } else {
      float num = NoZero();
      pixel.x += num;
      pixel.y += num;
      pixel.x += pixel.speedX;
      pixel.y += pixel.speedY;
    }

    DrawPixel(renderer, pixel.x, pixel.y, pixel.r, pixel.g, pixel.b, 0, 5);
  }
  Around(renderer);
  // ChoasOrb(renderer, 100, 100, 100);
}
