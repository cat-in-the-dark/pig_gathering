#ifndef HLAM_MATH_H
#define HLAM_MATH_H

#include <raylib.h>

#include <cmath>

namespace hlam {

// Don't add anything, just define those here so everything (incl. operators)
// is contained in our namespace
struct Vec2 : public ::Vector2 {};

struct Vec3 : public ::Vector3 {};

struct Rect : public ::Rectangle {};

struct Triangle2 {
  Vec2 p1;
  Vec2 p2;
  Vec2 p3;
};

struct Circle {
  Vec2 center;
  float radius;
};

inline bool operator==(Vec2 a, Vec2 b) { return (a.x == b.x) && (a.y == b.y); }
inline bool operator!=(Vec2 a, Vec2 b) { return !operator==(a, b); }

inline Vec2 operator+(Vec2 a) { return a; }
inline Vec2 operator-(Vec2 a) { return {-a.x, -a.y}; }

inline constexpr Vec2 operator+(Vec2 a, Vec2 b) { return {a.x + b.x, a.y + b.y}; }
inline Vec2 operator-(Vec2 a, Vec2 b) { return {a.x - b.x, a.y - b.y}; }

inline constexpr Vec2 operator*(Vec2 a, float scalar) { return {a.x * scalar, a.y * scalar}; }
inline Vec2 operator*(float scalar, Vec2 a) { return operator*(a, scalar); }

inline constexpr Vec2 operator/(Vec2 a, float scalar) { return operator*(a, 1.0f / scalar); }

/* Hadamard Product */
inline Vec2 operator*(Vec2 a, Vec2 b) { return {a.x * b.x, a.y * b.y}; }
inline Vec2 operator/(Vec2 a, Vec2 b) { return {a.x / b.x, a.y / b.y}; }

inline Vec2 &operator+=(Vec2 &a, Vec2 b) { return (a = a + b); }
inline Vec2 &operator-=(Vec2 &a, Vec2 b) { return (a = a - b); }
inline Vec2 &operator*=(Vec2 &a, float scalar) { return (a = a * scalar); }
inline Vec2 &operator/=(Vec2 &a, float scalar) { return (a = a / scalar); }

inline bool operator==(Vec3 a, Vec3 b) { return (a.x == b.x) && (a.y == b.y) && (a.z == b.z); }
inline bool operator!=(Vec3 a, Vec3 b) { return !operator==(a, b); }

inline Vec3 operator+(Vec3 a) { return a; }
inline Vec3 operator-(Vec3 a) { return {-a.x, -a.y, -a.z}; }

inline Vec3 operator+(Vec3 a, Vec3 b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
inline Vec3 operator-(Vec3 a, Vec3 b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }

inline Vec3 operator*(Vec3 a, float scalar) { return {a.x * scalar, a.y * scalar, a.z * scalar}; }
inline Vec3 operator*(float scalar, Vec3 a) { return operator*(a, scalar); }

inline Vec3 operator/(Vec3 a, float scalar) { return operator*(a, 1.0f / scalar); }

/* Hadamard Product */
inline Vec3 operator*(Vec3 a, Vec3 b) { return {a.x * b.x, a.y * b.y, a.z * b.z}; }
inline Vec3 operator/(Vec3 a, Vec3 b) { return {a.x / b.x, a.y / b.y, a.z / b.z}; }

inline Vec3 &operator+=(Vec3 &a, Vec3 b) { return (a = a + b); }
inline Vec3 &operator-=(Vec3 &a, Vec3 b) { return (a = a - b); }
inline Vec3 &operator*=(Vec3 &a, float scalar) { return (a = a * scalar); }
inline Vec3 &operator/=(Vec3 &a, float scalar) { return (a = a / scalar); }

inline float vec_dot(const Vec2 &v0, const Vec2 &v1) { return v0.x * v1.x + v0.y * v1.y; }
inline float vec_dot(const Vec3 &v0, const Vec3 &v1) { return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z; }

inline float vec_cross(const Vec2 &v0, const Vec2 &v1) { return v0.x * v1.y - v1.x * v0.y; }
inline Vec3 vec_cross(const Vec3 &v0, const Vec3 &v1) {
  return {
      v0.y * v1.z - v0.z * v1.y,
      v0.z * v1.x - v0.x * v1.z,
      v0.x * v1.y - v0.y * v1.x,
  };
}

inline float vec_length(const Vec2 &v) { return std::sqrt(v.x * v.x + v.y * v.y); }
inline float vec_length(const Vec3 &v) { return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

inline float vec_length_sqr(const Vec2 &v) { return v.x * v.x + v.y * v.y; }
inline float vec_length_sqr(const Vec3 &v) { return v.x * v.x + v.y * v.y + v.z * v.z; }

inline Vec2 vec_norm(const Vec2 &v) {
  Vec2 result{};
  auto length = vec_length(v);

  if (length > 0) {
    auto ilength = 1.0f / length;
    result.x = v.x * ilength;
    result.y = v.y * ilength;
  }

  return result;
}

inline float vec_dist(const Vec2 &v1, const Vec2 &v2) {
  return sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}

inline float vec_dist_sqr(const Vec2 &v1, const Vec2 &v2) {
  return ((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}

inline float vec_dist_sqr(const Vec3 &v1, const Vec3 &v2) {
  auto dx = v2.x - v1.x;
  auto dy = v2.y - v1.y;
  auto dz = v2.z - v1.z;
  return dx * dx + dy * dy + dz * dz;
}

}  // namespace hlam

#endif /* HLAM_MATH_H */

#ifdef HLAM_MATH_IMPLEMENTATION

namespace hlam {}  // namespace hlam

#endif /* HLAM_MATH_IMPLEMENTATION */