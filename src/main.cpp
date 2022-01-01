#include <vector>

#include "base/geometry.hpp"
#include "light.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include "tracer.hpp"

using geo::Vec3;
using std::vector;

int main() {
  const Material IVORY {1.0, {0.6, 0.3, 0.1, 0.0}, {0.4, 0.4, 0.3}, 50.};
  const Material GLASS {1.5, {0.0, 0.5, 0.1, 0.8}, {0.6, 0.7, 0.8}, 125.};
  const Material RED_RUBBER {1.0, {0.9, 0.1, 0.0, 0.0}, {0.3, 0.1, 0.1}, 10.};
  const Material MIRROR {1.0, {0.0, 10.0, 0.8, 0.0}, {1.0, 1.0, 1.0}, 1425.};

  vector<Sphere> spheres {
      Sphere {Vec3 {-3, 0, -16}, 2, IVORY},
      Sphere {Vec3 {-1.0, -1.5, -12}, 2, GLASS},
      Sphere {Vec3 {1.5, -0.5, -18}, 3, RED_RUBBER},
      Sphere {Vec3 {7, 5, -18}, 4, MIRROR},
  };

  vector<Light> lights {
      {{-20, 20, 20}, 1.5},
      {{30, 50, -25}, 1.8},
      {{30, 20, 30}, 1.7},
  };

  Render(spheres, lights);
  return 0;
}
