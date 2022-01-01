#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

#include "base/config.hpp"
#include "base/geometry.hpp"
#include "sphere.hpp"

using geo::Vec3;

Vec3 CastRay(const Vec3& origin, const Vec3& direction, const Sphere& sphere) {
  auto sphere_dist = std::numeric_limits<float>::max();
  if (!RaySphereIntersect(origin, direction, sphere, &sphere_dist)) {
    return Vec3{0.2, 0.7, 0.8};  // background color
  }
  return Vec3{0.4, 0.4, 0.3};
}

void Render(const Sphere& sphere) {
  std::vector<Vec3> frame_buf(WIDTH * HEIGHT);

  for (size_t j = 0; j < HEIGHT; ++j) {
    for (size_t i = 0; i < WIDTH; ++i) {
      auto x = (2 * (i + 0.5f) / WIDTH - 1) * tanf(FOV / 2.f) * WIDTH / HEIGHT;
      auto y = -(2 * (j + 0.5f) / HEIGHT - 1) * tanf(FOV / 2.f);
      auto origin = Vec3{0, 0, 0};
      auto direction = Vec3{x, y, -1}.normalize();
      frame_buf[i + j * WIDTH] = CastRay(origin, direction, sphere);
    }
  }

  std::ofstream ofs{OUT_FILE, std::ios::binary};
  ofs << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";
  for (size_t i = 0; i < HEIGHT * WIDTH; ++i) {
    for (size_t j = 0; j < 3; ++j) {
      ofs << static_cast<char>(
          255 * std::max(0.f, std::min(1.f, frame_buf[i][j])));
    }
  }
}

int main() {
  Sphere s1{Vec3{-6, 0, -16}, 2};

  Render(s1);
  return 0;
}
