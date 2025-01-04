#include "pig.h"

#include <raylib.h>
#include <raymath.h>

constexpr auto epsilon = 0.0001f;
constexpr auto pixelsInMeter = 8.0f;

constexpr auto kickAngle = PI / 6;

constexpr auto gravityAcceleration = 9.8f * pixelsInMeter;
constexpr auto airFriction = 0.03f * pixelsInMeter;
constexpr auto groundFriction = 10.0f * pixelsInMeter;

Pig::Pig(hlam::Vec2 pos) : elevationSpeed_(0.0f), Entity(pos, pigWitdh, pigHeight) {}

void Pig::DoKick(Kick kick) {
  elevationSpeed_ = kick.impulse * sinf(kickAngle);
  auto horizontalSpeed = kick.impulse * cosf(kickAngle);

  speed = kick.dir * horizontalSpeed;
}

void Pig::Update(float dt) {
  // vertical update
  if (elevationSpeed_ > 0.0f) {
  }

  if (elevation > 0.0f) {
    elevation += elevationSpeed_ * dt;
    elevationSpeed_ -= gravityAcceleration * dt;
  } else {
    elevationSpeed_ = 0;
  }

  if (elevation < 0) {
    elevation = 0;
  }

  // horizontal update
  if (hlam::vec_length_sqr(speed) > 0.0f) {
    auto friction = groundFriction * dt;
    if (elevation > 0.0f) {
      friction = airFriction * dt;
    }

    auto dir = atan2f(speed.y, speed.x);
    speed -= hlam::vec_norm(speed) * friction;

    // check if friction overshoot
    auto newDir = atan2f(speed.y, speed.x);
    if (fabsf(dir - newDir) > epsilon) {
      speed = {0, 0};
    }
  }

  pos += speed;
}

void Pig::Draw() {
  // draw elevation

  DrawEllipseLines(pos.x, pos.y, width / 2, height / 2, WHITE);
}
