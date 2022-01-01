#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

#include "base/config.hpp"
#include "base/geometry.hpp"
#include "material.hpp"
#include "sphere.hpp"

using geo::Vec3;

bool SceneIntersect(
    const Vec3& origin,
    const Vec3& direction,
    const std::vector<Sphere>& spheres,
    Vec3* p_hit,
    Vec3* p_n,
    Material* p_material) {
  auto spheres_dist = std::numeric_limits<float>::max();
  for (const auto& sphere : spheres) {
    auto sphere_dist_i = std::numeric_limits<float>::max();
    if (RaySphereIntersect(origin, direction, sphere, &sphere_dist_i) &&
        sphere_dist_i < spheres_dist) {
      spheres_dist = sphere_dist_i;
      *p_hit = origin + direction * sphere_dist_i;
      *p_n = (*p_hit - sphere.center).normalize();
      *p_material = sphere.material;
    }
  }
  return spheres_dist < MAX_DIST;
}

Vec3 CastRay(
    const Vec3& origin,
    const Vec3& direction,
    const std::vector<Sphere>& spheres) {
  Material material;
  Vec3 hit;
  Vec3 n;

  if (!SceneIntersect(origin, direction, spheres, &hit, &n, &material)) {
    return Vec3{0.2, 0.7, 0.8};  // background color
  }
  return material.diffuse_color;
}

void Render(const std::vector<Sphere>& spheres) {
  std::vector<Vec3> frame_buf(WIDTH * HEIGHT);

  for (size_t j = 0; j < HEIGHT; ++j) {
    for (size_t i = 0; i < WIDTH; ++i) {
      auto x = (2 * (i + 0.5f) / WIDTH - 1) * tanf(FOV / 2.f) * WIDTH / HEIGHT;
      auto y = -(2 * (j + 0.5f) / HEIGHT - 1) * tanf(FOV / 2.f);
      auto origin = Vec3{0, 0, 0};
      auto direction = Vec3{x, y, -1}.normalize();
      frame_buf[i + j * WIDTH] = CastRay(origin, direction, spheres);
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
  const Material IVORY{{0.4, 0.4, 0.3}};
  const Material GLASS{{0.6, 0.7, 0.8}};
  const Material RED_RUBBER{{0.3, 0.1, 0.1}};
  const Material MIRROR{{1.0, 1.0, 1.0}};

  std::vector<Sphere> spheres = {
      Sphere{Vec3{-3, 0, -16}, 2, IVORY},
      Sphere{Vec3{-1.0, -1.5, -12}, 2, GLASS},
      Sphere{Vec3{1.5, -0.5, -18}, 3, RED_RUBBER},
      Sphere{Vec3{7, 5, -18}, 4, MIRROR},
  };

  Render(spheres);
  return 0;
}
