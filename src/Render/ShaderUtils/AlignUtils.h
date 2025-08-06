#pragma once

#include <glm/glm.hpp>
#include <type_traits>

namespace ShaderUtils {
#define STD140(type, name) std140_aligned<type> name

template <typename T, typename Derived>
struct std140_ops {
  T value;

  explicit std140_ops(const T& val) : value(val) {}
  std140_ops() = default;

  Derived& operator=(const T& val) {
    value = val;
    return static_cast<Derived&>(*this);
  }

  Derived& operator+=(const T& rhs) {
    value += rhs;
    return static_cast<Derived&>(*this);
  }
  Derived& operator-=(const T& rhs) {
    value -= rhs;
    return static_cast<Derived&>(*this);
  }
  Derived& operator*=(const T& rhs) {
    value *= rhs;
    return static_cast<Derived&>(*this);
  }
  Derived& operator/=(const T& rhs) {
    value /= rhs;
    return static_cast<Derived&>(*this);
  }

  T operator+(const T& rhs) const { return value + rhs; }
  T operator-(const T& rhs) const { return value - rhs; }
  T operator*(const T& rhs) const { return value * rhs; }
  T operator/(const T& rhs) const { return value / rhs; }

  operator T&() { return value; }
  operator const T&() const { return value; }
};

template <typename T>
struct std140_aligned : std140_ops<T, std140_aligned<T>> {
  using std140_ops<T, std140_aligned<T>>::std140_ops;
};

template <>
struct alignas(4) std140_aligned<float>
    : std140_ops<float, std140_aligned<float>> {
  using std140_ops::std140_ops;
};

template <>
struct alignas(4) std140_aligned<int> : std140_ops<int, std140_aligned<int>> {
  using std140_ops::std140_ops;
};

template <>
struct alignas(8) std140_aligned<glm::vec2>
    : std140_ops<glm::vec2, std140_aligned<glm::vec2>> {
  using std140_ops::std140_ops;
};

template <>
struct alignas(16) std140_aligned<glm::vec3>
    : std140_ops<glm::vec3, std140_aligned<glm::vec3>> {
  float _pad = 0.0f;
  explicit std140_aligned(const glm::vec3& val)
      : std140_ops<glm::vec3, std140_aligned<glm::vec3>>(val) {}
  std140_aligned() = default;

  using std140_ops::operator=;
};

template <>
struct alignas(16) std140_aligned<glm::vec4>
    : std140_ops<glm::vec4, std140_aligned<glm::vec4>> {
  using std140_ops::std140_ops;
};

template <>
struct alignas(16) std140_aligned<glm::mat4>
    : std140_ops<glm::mat4, std140_aligned<glm::mat4>> {
  using std140_ops::std140_ops;
};

static_assert(sizeof(std140_aligned<glm::vec3>) == 16,
              "vec3 must occupy 16 bytes in std140");
static_assert(alignof(std140_aligned<glm::vec4>) == 16,
              "vec4 must be 16-byte aligned");
static_assert(alignof(std140_aligned<glm::mat4>) == 16,
              "mat4 must be 16-byte aligned");
}  // namespace ShaderUtils