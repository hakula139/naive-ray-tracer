#include <vector>

#include "base/geometry.hpp"
#include "light.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include "tracer.hpp"

using geo::Vec3;
using std::vector;

int main() {
  const Material RED_RUBBER {1.0, {0.9, 0.1, 0.0, 0.0}, {0.3, 0.1, 0.1}, 10.};
  const Material BLUE_RUBBER {1.0, {0.9, 0.1, 0.0, 0.0}, {0.1, 0.1, 0.3}, 10.};
  const Material IVORY {1.0, {0.6, 0.3, 0.1, 0.0}, {0.4, 0.4, 0.3}, 50.};
  const Material GLASS {1.5, {0.0, 1.0, 0.1, 0.8}, {0.6, 0.7, 0.8}, 125.};
  const Material MIRROR {1.0, {0.0, 9.5, 0.8, 0.0}, {1.0, 1.0, 1.0}, 1425.};

  vector<Sphere> spheres {
      Sphere {Vec3 {4.5, -1.5, -11}, 1.5, RED_RUBBER},
      Sphere {Vec3 {7, -1, -9}, 1.5, BLUE_RUBBER},
      Sphere {Vec3 {-9, 0, -12}, 2, BLUE_RUBBER},
      Sphere {Vec3 {-5, -2, -8}, 1.5, IVORY},
      Sphere {Vec3 {-3, 0, -16}, 2, IVORY},
      Sphere {Vec3 {-1.0, -1.5, -10}, 2, GLASS},
      Sphere {Vec3 {7, 5, -18}, 4, GLASS},
      Sphere {Vec3 {-9, 6, -21}, 5, MIRROR},
      Sphere {Vec3 {1.5, -0.5, -19}, 3, MIRROR},
  };

  vector<Light> lights {
      {{-40, -10, 20}, 1.5},
      {{0, 10, 50}, 1.2},
      {{-10, 70, -25}, 1.9},
      {{30, 20, 40}, 1.7},
  };

  Render(spheres, lights);
  return 0;
}
