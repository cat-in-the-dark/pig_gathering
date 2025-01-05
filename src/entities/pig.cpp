#include "pig.h"

#include <HLAM/textures.h>
#include <raylib.h>

#include <algorithm>

#include "const.h"

constexpr auto epsilon = 0.0001f;
constexpr auto pixelsInMeter = 8.0f;

constexpr auto kickAngle = PI / 4;

constexpr auto gravityAcceleration = 9.8f / pixelsInMeter;
constexpr auto airFriction = 0.03f * pixelsInMeter;
constexpr auto groundFriction = 10.0f * pixelsInMeter;

constexpr hlam::Vec2 shadowYOffset = {0.0f, 10.0f};

Texture generateShadow() {
  auto radius = Pig::pigWitdh / 2.0f;
  auto img = GenImageDither(radius * 2, radius * 2, BLACK, WHITE, 4);
  auto mask = GenImageColor(radius * 2, radius * 4, {0, 0, 0, 0});
  // TODO: ellipse
  ImageDrawCircle(&mask, radius, radius, radius, WHITE);
  ImageResizeNN(&mask, radius * 2, radius * 2);
  ImageAlphaMask(&img, mask);
  ClearAlphaColor(img, BLACK);
  auto res = LoadTextureFromImage(img);
  UnloadImage(img);
  UnloadImage(mask);
  return res;
}

Pig::Pig(hlam::Vec2 pos)
    : Entity(pos, pigSize),
      speed{0, 0},
      elevation(0),
      isDead(false),
      elevationSpeed_(0.0f),
      state_(State::IDLE),
      shadow_(generateShadow()) {}

Pig::~Pig() {
  UnloadTexture(shadow_);
}

void Pig::DoKick(Kick kick) {
  elevationSpeed_ = kick.impulse * sinf(kickAngle);
  auto horizontalSpeed = kick.impulse * cosf(kickAngle);

  speed = kick.dir * horizontalSpeed;
  state_ = State::KICKED;
}

void Pig::Update(float dt) {
  if (isDead) {
    return;
  }

  dt *= 3;
  if (state_ == State::KICKED) {
    if (elevation >= 0.0f) {
      elevation += elevationSpeed_ * dt;
      elevationSpeed_ -= gravityAcceleration * dt;
    }

    if (elevation < 0.0f) {
      state_ = State::IDLE;
      elevationSpeed_ = 0.0f;
      elevation = 0.0f;
    }
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
    } else {
      // Check world boundaries
      auto newPos = pos + speed;
      if (newPos.x + pigWitdh > kWorldPosRight || newPos.x - pigWitdh < kWorldPosLeft) {
        speed.x *= -1;
      }
      if (newPos.y + pigHeight > kWorldPosDown || newPos.y - pigHeight < kWorldPosUp) {
        speed.y *= -1;
      }
    }
  }

  pos += speed;
}

void Pig::Draw() {
  if (isDead) {
    return;
  }

  if (elevation > 0) {
    DrawTextureV(shadow_, pos - drawDelta + shadowYOffset, WHITE);
  }

  DrawEllipse(pos.x, pos.y - elevation * 50, size.x / 2, size.y / 2, BLACK);
  DrawEllipseLines(pos.x, pos.y - elevation * 50, size.y / 2, size.x / 2, WHITE);
}

void Pig::Kidnapped() {
  state_ = State::KIDNAPPED;
}
