#include "geometry.hpp"

namespace geo {

Vec3 cross(Vec3 v1, Vec3 v2) {
  return {
      v1.y * v2.z - v1.z * v2.y,
      v1.z * v2.x - v1.x * v2.z,
      v1.x * v2.y - v1.y * v2.x,
  };
}

}  // namespace geo
