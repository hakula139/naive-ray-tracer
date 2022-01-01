#include "tracer.hpp"

#include <algorithm>
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
using std::vector;

Vec3 Reflect(const Vec3& i, const Vec3& n) { return i - n * 2.f * (i * n); }

bool SceneIntersect(
    const Vec3& origin,
    const Vec3& dir,
    const vector<Sphere>& spheres,
    Vec3* p_hit = nullptr,
    Vec3* p_n = nullptr,
    Material* p_material = nullptr) {
  auto spheres_dist = std::numeric_limits<float>::max();
  for (const auto& sphere : spheres) {
    auto sphere_dist_i = std::numeric_limits<float>::max();
    if (RaySphereIntersect(origin, dir, sphere, &sphere_dist_i) &&
        sphere_dist_i < spheres_dist) {
      spheres_dist = sphere_dist_i;
      auto hit = origin + dir * sphere_dist_i;
      if (p_hit) *p_hit = hit;
      if (p_n) *p_n = (hit - sphere.center).normalize();
      if (p_material) *p_material = sphere.material;
    }
  }
  return spheres_dist < MAX_DIST;
}

Vec3 CastRay(
    const Vec3& origin,
    const Vec3& dir,
    const vector<Sphere>& spheres,
    const vector<Light>& lights,
    size_t depth = 0) {
  Vec3 hit;
  Vec3 n;
  Material material;

  if (depth > MAX_DEPTH ||
      !SceneIntersect(origin, dir, spheres, &hit, &n, &material)) {
    return Vec3 {0.2, 0.7, 0.8};  // background color
  }

  auto reflect_dir = Reflect(dir, n).normalize();
  auto reflect_color = CastRay(hit, reflect_dir, spheres, lights, depth + 1);

  auto diffuse_light_intensity = 0.f;
  auto specular_light_intensity = 0.f;

  for (const auto& light : lights) {
    auto light_dir = (light.position - hit).normalize();
    Vec3 shadow_hit;

    if (SceneIntersect(hit, light_dir, spheres, &shadow_hit) &&
        (shadow_hit - hit).norm() < (light.position - hit).norm()) {
      continue;
    }

    auto diffuse_rate = std::max(0.f, light_dir * n);
    diffuse_light_intensity += light.intensity * diffuse_rate;

    auto specular_rate = powf(
        std::max(0.f, Reflect(light_dir, n) * dir), material.specular_exponent);
    specular_light_intensity += light.intensity * specular_rate;
  }

  return material.diffuse_color * diffuse_light_intensity * material.albedo[0] +
         Vec3 {1., 1., 1.} * specular_light_intensity * material.albedo[1] +
         reflect_color * material.albedo[2];
}

void Render(const vector<Sphere>& spheres, const vector<Light>& lights) {
  vector<Vec3> frame_buf(WIDTH * HEIGHT);

  for (size_t j = 0; j < HEIGHT; ++j) {
    for (size_t i = 0; i < WIDTH; ++i) {
      auto x = (2 * (i + 0.5f) / WIDTH - 1) * tanf(FOV / 2.f) * WIDTH / HEIGHT;
      auto y = -(2 * (j + 0.5f) / HEIGHT - 1) * tanf(FOV / 2.f);
      auto origin = Vec3 {0, 0, 0};
      auto dir = Vec3 {x, y, -1}.normalize();
      frame_buf[i + j * WIDTH] = CastRay(origin, dir, spheres, lights);
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
