#ifndef HLAM_COLLISIONS2D_H
#define HLAM_COLLISIONS2D_H

#include <algorithm>
#include <cmath>

#include "hlam_math.h"

namespace hlam {

namespace {

constexpr auto epsilon = 1.192092896e-07F;  // smallest such that 1.0+epsilon != 1.0

inline float triangle_sign(const Vec2& p1, const Vec2& p2, const Vec2& p3) {
  return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

inline bool point_in_triangle(const Triangle2& t, const Vec2& center) {
  auto d1 = triangle_sign(center, t.p1, t.p2);
  auto d2 = triangle_sign(center, t.p2, t.p3);
  auto d3 = triangle_sign(center, t.p3, t.p1);
  auto hasNeg = d1 < 0 || d2 < 0 || d3 < 0;
  auto hasPos = d1 > 0 || d2 > 0 || d3 > 0;
  return !(hasNeg && hasPos);
}

};  // namespace

inline bool collision_line_circle(const Vec2& p1, const Vec2& p2, const Vec2& center, float radius) {
  auto temp = p2 - p1;
  auto temp1 = center - p1;
  auto len = vec_length(temp);
  auto norm = vec_norm(temp);
  auto dot = vec_dot(temp1, norm);
  Vec2 res{};
  if (dot <= 0) {
    res = p1;
  } else if (dot >= len) {
    res = p2;
  } else {
    res = norm * dot + p1;
  }

  res = center - res;

  return vec_length(res) <= radius;
}

inline bool collision_triangle_circle(const Triangle2& t, const Vec2& center, float radius) {
  // 1. if any of triangle vertices is inside the circle => intersection
  auto radius2 = radius * radius;
  if (vec_dist_sqr(t.p1, center) < radius2 || vec_dist_sqr(t.p2, center) < radius2 ||
      vec_dist_sqr(t.p3, center) < radius2) {
    return true;
  }

  // 2. if circle center is inside the triangle => intersection
  if (point_in_triangle(t, center)) {
    return true;
  }

  // 3. If any of line intersects with the circle => intersection
  if (collision_line_circle(t.p1, t.p2, center, radius) || collision_line_circle(t.p2, t.p3, center, radius) ||
      collision_line_circle(t.p3, t.p1, center, radius)) {
    return true;
  }

  return false;
}

// Check the collision between two lines defined by two points each, returns collision point by reference
inline bool collision_line_line(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Vec2& p4, Vec2& collisionPoint) {
  bool collision = false;

  float div = (p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y);

  if (fabsf(div) >= epsilon) {
    collision = true;

    float xi = ((p3.x - p4.x) * (p1.x * p2.y - p1.y * p2.x) - (p1.x - p2.x) * (p3.x * p4.y - p3.y * p4.x)) / div;
    float yi = ((p3.y - p4.y) * (p1.x * p2.y - p1.y * p2.x) - (p1.y - p2.y) * (p3.x * p4.y - p3.y * p4.x)) / div;

    if (((fabsf(p1.x - p2.x) > epsilon) && (xi < fminf(p1.x, p2.x) || (xi > fmaxf(p1.x, p2.x)))) ||
        ((fabsf(p3.x - p4.x) > epsilon) && (xi < fminf(p3.x, p4.x) || (xi > fmaxf(p3.x, p4.x)))) ||
        ((fabsf(p1.y - p2.y) > epsilon) && (yi < fminf(p1.y, p2.y) || (yi > fmaxf(p1.y, p2.y)))) ||
        ((fabsf(p3.y - p4.y) > epsilon) && (yi < fminf(p3.y, p4.y) || (yi > fmaxf(p3.y, p4.y)))))
      collision = false;

    if (collision) {
      collisionPoint.x = xi;
      collisionPoint.y = yi;
    }
  }

  return collision;
}

inline bool collision_rect_rect(const Rect& r1, const Rect& r2) {
  return (r1.x + r1.width > r2.x) && (r2.x + r2.width > r1.x) && (r1.y + r1.height > r2.y) && (r2.y + r2.height > r1.y);
}

inline bool collision_line_rect(const Vec2& p1, const Vec2& p2, const Rect& r) {
  Rect boundingRect = {fminf(p1.x, p2.x), fminf(p1.y, p2.y), fabsf(p1.x - p2.x), fabsf(p1.y - p2.y)};

  if (!collision_rect_rect(boundingRect, r)) {
    return false;
  }

  Vec2 collisionPoint{};
  bool left = collision_line_line(p1, p2, {r.x, r.y}, {r.x, r.y + r.height}, collisionPoint);
  bool right = collision_line_line(p1, p2, {r.x + r.width, r.y}, {r.x + r.width, r.y + r.height}, collisionPoint);
  bool top = collision_line_line(p1, p2, {r.x, r.y}, {r.x + r.width, r.y}, collisionPoint);
  bool bottom = collision_line_line(p1, p2, {r.x, r.y + r.height}, {r.x + r.width, r.y + r.height}, collisionPoint);

  // if ANY of the above are true, the line
  // has hit the rectangle
  if (left || right || top || bottom) {
    return true;
  }
  return false;
}

inline Vec2 collision_point_circle_rectangle(Circle circle, Rect rect) {
  auto res = hlam::Vec2{std::clamp(circle.center.x, rect.x, rect.x + rect.width),
                        std::clamp(circle.center.y, rect.y, rect.y + rect.height)};

  // Check if point is inside the rectangle
  auto inside = res.x > rect.x && res.x < rect.x + rect.width && res.y > rect.y && res.y < rect.y + rect.height;
  if (inside) {
    // project the point on the edge
    auto leftDist = res.x - rect.x;
    auto rightDist = rect.x + rect.width - res.x;
    auto topDist = res.y - rect.y;
    auto bottomDist = rect.y + rect.width - res.y;

    auto nearestDist = std::min({leftDist, rightDist, topDist, bottomDist});
    // It should be OK to compare floats directly here as we just found minimum and didn't do any arithmetics on them
    if (nearestDist == leftDist) {
      res.x = rect.x;
    } else if (nearestDist == rightDist) {
      res.x = rect.x + rect.width;
    } else if (nearestDist == topDist) {
      res.y = rect.y;
    } else if (nearestDist == bottomDist) {
      res.y = rect.y + rect.height;
    }
  }

  return res;
}

inline Vec2 fit_in_bounds(Vec2 pos, Vec2 size, Rect world) {
  if (pos.x + size.x / 2 > world.width) {
    pos.x = world.width - size.x / 2;
  } else if (pos.x - size.x / 2 < world.x) {
    pos.x = world.x + size.x / 2;
  }

  if (pos.y + size.y / 2 > world.height) {
    pos.y = world.height - size.y / 2;
  } else if (pos.y - size.y / 2 < world.y) {
    pos.y = world.y + size.y / 2;
  }

  return pos;
}
}  // namespace hlam

#endif  // HLAM_COLLISIONS2D_H

#ifndef HLAM_COLLISIONS2D_IMPLEMENTATION
namespace hlam {}
#endif
