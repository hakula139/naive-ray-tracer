#ifndef SRC_TRACER_HPP_
#define SRC_TRACER_HPP_

#include <vector>

#include "light.hpp"
#include "sphere.hpp"

void Render(
    const std::vector<Sphere>& spheres, const std::vector<Light>& lights);

#endif  // SRC_TRACER_HPP_
