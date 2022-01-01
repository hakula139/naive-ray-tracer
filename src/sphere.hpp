#ifndef SRC_SPHERE_HPP_
#define SRC_SPHERE_HPP_

#include "base/geometry.hpp"
#include "material.hpp"

struct Sphere {
  geo::Vec3 center;
  float radius;
  Material material;
};

bool RaySphereIntersect(
    const geo::Vec3& origin,
    const geo::Vec3& direction,
    const Sphere& s,
    float* p_t0);

#endif  // SRC_SPHERE_HPP_
