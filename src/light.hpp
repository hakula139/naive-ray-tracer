#ifndef SRC_LIGHT_HPP_
#define SRC_LIGHT_HPP_

#include "base/geometry.hpp"

struct Light {
  geo::Vec3 position;
  float intensity;
};

#endif  // SRC_LIGHT_HPP_
