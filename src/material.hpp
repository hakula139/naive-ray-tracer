#ifndef SRC_MATERIAL_HPP_
#define SRC_MATERIAL_HPP_

#include "base/geometry.hpp"

struct Material {
  float refractive_index = 1;
  geo::Vec4 albedo = {1, 0, 0, 0};
  geo::Vec3 diffuse_color = {0, 0, 0};
  float specular_exponent = 0;
};

#endif  // SRC_MATERIAL_HPP_
