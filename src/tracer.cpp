#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

#include "base/config.hpp"
#include "base/geometry.hpp"

using geo::Vec3;

void render() {
  std::vector<Vec3> frame_buf(WIDTH * HEIGHT);

  for (size_t j = 0; j < HEIGHT; ++j) {
    for (size_t i = 0; i < WIDTH; ++i) {
      frame_buf[i + j * WIDTH] = Vec3{j / HEIGHT, i / WIDTH, 0.f};
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
  render();
  return 0;
}
