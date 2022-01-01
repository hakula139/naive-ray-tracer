#ifndef SRC_BASE_GEOMETRY_HPP_
#define SRC_BASE_GEOMETRY_HPP_

#include <cassert>
#include <cmath>
#include <iostream>

namespace geo {

template <size_t DIM>
struct Vector {
  float& operator[](const size_t i) {
    assert(i < DIM);
    return data[i];
  }

  const float& operator[](const size_t i) const {
    assert(i < DIM);
    return data[i];
  }

  float data[DIM] = {};
};

template <size_t DIM>
Vector<DIM> operator*(const Vector<DIM>& lhs, const float rhs) {
  Vector<DIM> ret;
  for (size_t i = DIM; i--; ret[i] = lhs[i] * rhs) {}
  return ret;
}

template <size_t DIM>
float operator*(const Vector<DIM>& lhs, const Vector<DIM>& rhs) {
  float ret = 0;
  for (size_t i = DIM; i--; ret += lhs[i] * rhs[i]) {}
  return ret;
}

template <size_t DIM>
Vector<DIM> operator+(Vector<DIM> lhs, const Vector<DIM>& rhs) {
  for (size_t i = DIM; i--; lhs[i] += rhs[i]) {}
  return lhs;
}

template <size_t DIM>
Vector<DIM> operator-(Vector<DIM> lhs, const Vector<DIM>& rhs) {
  for (size_t i = DIM; i--; lhs[i] -= rhs[i]) {}
  return lhs;
}

template <size_t DIM>
Vector<DIM> operator-(const Vector<DIM>& lhs) {
  return lhs * -1;
}

template <>
struct Vector<3> {
  float& operator[](const size_t i) {
    assert(i < 3);
    return i == 0 ? x : (i == 1 ? y : z);
  }

  const float& operator[](const size_t i) const {
    assert(i < 3);
    return i == 0 ? x : (i == 1 ? y : z);
  }

  float norm() const { return sqrtf(x * x + y * y + z * z); }

  Vector<3>& normalize(float l = 1) {
    *this = (*this) * (l / norm());
    return *this;
  }

  float x = 0;
  float y = 0;
  float z = 0;
};

using Vec3 = Vector<3>;
using Vec4 = Vector<4>;

Vec3 cross(Vec3 v1, Vec3 v2);

template <size_t DIM>
std::ostream& operator<<(std::ostream& out, const Vector<DIM>& v) {
  for (size_t i = 0; i < DIM; ++i) out << v[i] << " ";
  return out;
}

}  // namespace geo

#endif  // SRC_BASE_GEOMETRY_HPP_
