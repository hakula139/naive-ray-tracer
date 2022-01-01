#include "sphere.hpp"

#include <cmath>
#include <utility>  // std::swap

#include "base/config.hpp"
#include "base/geometry.hpp"

using geo::Vec3;

// Check the following link for a detailed description of the algorithm.
// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
bool RaySphereIntersect(
    const Vec3& origin, const Vec3& direction, const Sphere& s, float* p_t0) {
  auto l = s.center - origin;
  auto t_ca = l * direction;
  auto d__2 = l * l - t_ca * t_ca;
  if (d__2 > s.radius * s.radius) return false;

  auto t_hc = sqrtf(s.radius * s.radius - d__2);
  auto& t0 = *p_t0;
  t0 = t_ca - t_hc;
  auto t1 = t_ca + t_hc;

  if (t0 > t1) std::swap(t0, t1);
  if (t0 < EPS) t0 = t1;
  return t0 >= EPS;
}
