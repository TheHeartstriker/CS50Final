#include "SpaceS.h"

#include <array>
#include <random>
#include <vector>

#include "../Shapes/Shapes.h"
// So we can track the window size
extern int Winwidth;
extern int Winheight;
static int intialWidth = Winwidth;
static int intialHeight = Winheight;
// Center circle radius and location
static const int radius = 100;
static int LocatinX = Winwidth / 2;
static int LocatinY = Winheight / 2;
static int circlePixels = 2500;
// Random generators
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(0, Winwidth);
std::uniform_int_distribution<Uint8> dis_Color(0, 255);
std::uniform_real_distribution<float> speed_dis(-3, 3);
std::uniform_real_distribution<float> point_speed(0.4, 1.4);
std::uniform_int_distribution<int> orbit_distance(200, 800);
//-3 to 3 but not 0
static float NoZero() {
  int num = speed_dis(gen);
  while (num == 0) {
    num = speed_dis(gen);
  }
  return num;
}

// Defintions
// Pixel objects
namespace {
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

}  // namespace
// Point objects
namespace {
struct Point {
  // Location
  int x;
  int y;
  float speed;
  // Circluar array of points
  std::vector<Point> Path;
  // Counter for the path
  float PathCounter = 0;
  Uint8 r;
  Uint8 g;
  Uint8 b;
};
}  // namespace

// Array of data for the orbit and pixels
static std::vector<Pixel> pixels;
static std::vector<Point> points;
// Functions
static std::vector<Point> generateCircularPath(int centerX, int centerY,
                                               int radius, int numPoints);
static void MoveCloser(int& x2, int& y2, int x, int y, int units);

// Intalize the pixels for center ball with starting data
static void initPixels(std::vector<Pixel>& pixels, int radius, int cx, int cy,
                       int Amount) {
  if (Winheight) {
    // Random valid x and y range
    std::uniform_real_distribution<float> angle_dist(0.0f, 2.0f * M_PI);
    std::uniform_real_distribution<float> radius_dist(
        0.0f, static_cast<float>(radius));

    for (int i = 0; i < Amount; i++) {
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
// Intalize the points with starting data
static void initPoints(std::vector<Point>& points, int MidX, int MidY,
                       int Orbits) {
  std::vector<std::vector<Point>> path;
  // Create mutiple paths and push them to path
  for (int i = 0; i < Orbits; ++i) {
    path.push_back(generateCircularPath(MidX, MidY, orbit_distance(gen), 1000));
  }
  for (int i = 0; i < Orbits; ++i) {
    Point point;
    point.x = path[i][0].x;
    point.y = path[i][0].y;
    point.Path = path[i];
    point.PathCounter = 0;
    point.speed = point_speed(gen);
    point.r = dis_Color(gen);
    point.g = dis_Color(gen);
    point.b = dis_Color(gen);
    points.push_back(point);
  }
}
// Helper functions
//  Generate a circular path of points
static std::vector<Point> generateCircularPath(int centerX, int centerY,
                                               int radius, int numPoints) {
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

// Move a point closer to another point
static void MoveCloser(int& x2, int& y2, int x, int y, int units) {
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
// Handle window change
static void CheckWindowChange() {
  if (Winwidth != intialWidth || Winheight != intialHeight) {
    pixels.clear();
    intialWidth = Winwidth;
    intialHeight = Winheight;
    LocatinX = Winwidth / 2;
    LocatinY = Winheight / 2;
    initPixels(pixels, radius, LocatinX, LocatinY, circlePixels);
    points.clear();
    initPoints(points, LocatinX, LocatinY, 25);
  }
}
// Draw functions
static void DrawPoints(SDL_Renderer* renderer, std::vector<Point>& points) {
  for (auto& point : points) {
    point.PathCounter += point.speed;
    if (point.PathCounter >= point.Path.size()) {
      point.PathCounter -= point.Path.size();
    }
    int index = static_cast<int>(point.PathCounter);
    point.x = point.Path[index].x;
    point.y = point.Path[index].y;
    DrawPixel(renderer, point.x, point.y, point.r, point.g, point.b, 0, 5);
  }
}

static void DrawPixels(SDL_Renderer* renderer, std::vector<Pixel>& pixels) {
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
      // Random movement
      float num1 = NoZero();
      float num2 = NoZero();
      pixel.x += num1;
      pixel.y += num2;
      // Speed movement
      pixel.x += pixel.speedX;
      pixel.y += pixel.speedY;
    }
    DrawPixel(renderer, pixel.x, pixel.y, pixel.r, pixel.g, pixel.b, 0, 5);
  }
}
// Main render call
void MainSpacesCall(SDL_Renderer* renderer) {
  if (pixels.empty()) {
    initPixels(pixels, radius, LocatinX, LocatinY, circlePixels);
  }
  if (points.empty()) {
    initPoints(points, LocatinX, LocatinY, 25);
  }
  CheckWindowChange();
  DrawPixels(renderer, pixels);
  DrawPoints(renderer, points);
}
