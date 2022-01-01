#ifndef SRC_BASE_CONFIG_HPP_
#define SRC_BASE_CONFIG_HPP_

#include <cmath>

constexpr float WIDTH = 1920;
constexpr float HEIGHT = 1080;
constexpr float FOV = M_PI / 3.f;  // field of view
constexpr float EPS = 1e-3f;
constexpr float MAX_DIST = 1e3f;

constexpr const char* OUT_FILE = "output/out.ppm";

#endif  // SRC_BASE_CONFIG_HPP_
