#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

#include "base/config.hpp"
#include "base/geometry.hpp"
#include "light.hpp"
#include "material.hpp"
#include "sphere.hpp"

using geo::Vec3;

Vec3 Reflect(const Vec3& i, const Vec3& n) { return i - n * 2.f * (i * n); }

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
    const std::vector<Sphere>& spheres,
    const std::vector<Light>& lights) {
  Material material;
  Vec3 hit;
  Vec3 n;

  if (!SceneIntersect(origin, direction, spheres, &hit, &n, &material)) {
    return Vec3 {0.2, 0.7, 0.8};  // background color
  }

  auto diffuse_light_intensity = 0.f;
  auto specular_light_intensity = 0.f;

  for (const auto& light : lights) {
    auto light_direction = (light.position - hit).normalize();
    auto diffuse_rate = std::max(0.f, light_direction * n);
    diffuse_light_intensity += light.intensity * diffuse_rate;
    auto specular_rate = powf(
        std::max(0.f, Reflect(light_direction, n) * direction),
        material.specular_exponent);
    specular_light_intensity += light.intensity * specular_rate;
  }

  return material.diffuse_color * diffuse_light_intensity * material.albedo[0] +
         Vec3 {1, 1, 1} * specular_light_intensity * material.albedo[1];
}

void Render(
    const std::vector<Sphere>& spheres, const std::vector<Light>& lights) {
  std::vector<Vec3> frame_buf(WIDTH * HEIGHT);

  for (size_t j = 0; j < HEIGHT; ++j) {
    for (size_t i = 0; i < WIDTH; ++i) {
      auto x = (2 * (i + 0.5f) / WIDTH - 1) * tanf(FOV / 2.f) * WIDTH / HEIGHT;
      auto y = -(2 * (j + 0.5f) / HEIGHT - 1) * tanf(FOV / 2.f);
      auto origin = Vec3 {0, 0, 0};
      auto direction = Vec3 {x, y, -1}.normalize();
      frame_buf[i + j * WIDTH] = CastRay(origin, direction, spheres, lights);
    }
  }

  std::ofstream ofs {OUT_FILE, std::ios::binary};
  ofs << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";
  for (const auto& c : frame_buf) {
    for (size_t j = 0; j < 3; ++j) {
      // Prevents overflow.
      ofs << static_cast<char>(std::max(0.f, std::min(1.f, c[j])) * 255);
    }
  }
}

int main() {
  const Material IVORY {1.0, {0.6, 0.3, 0.1, 0.0}, {0.4, 0.4, 0.3}, 50.};
  const Material GLASS {1.5, {0.0, 0.5, 0.1, 0.8}, {0.6, 0.7, 0.8}, 125.};
  const Material RED_RUBBER {1.0, {0.9, 0.1, 0.0, 0.0}, {0.3, 0.1, 0.1}, 10.};
  const Material MIRROR {1.0, {0.0, 10.0, 0.8, 0.0}, {1.0, 1.0, 1.0}, 1425.};

  std::vector<Sphere> spheres {
      Sphere {Vec3 {-3, 0, -16}, 2, IVORY},
      Sphere {Vec3 {-1.0, -1.5, -12}, 2, GLASS},
      Sphere {Vec3 {1.5, -0.5, -18}, 3, RED_RUBBER},
      Sphere {Vec3 {7, 5, -18}, 4, MIRROR},
  };

  std::vector<Light> lights {
      {{-20, 20, 20}, 1.5},
      {{30, 50, -25}, 1.8},
      {{30, 20, 30}, 1.7},
  };

  Render(spheres, lights);
  return 0;
}
