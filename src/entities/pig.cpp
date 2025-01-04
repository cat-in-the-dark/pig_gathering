#include "pig.h"

#include <HLAM/textures.h>
#include <raylib.h>

#include <iostream>

constexpr auto epsilon = 0.0001f;
constexpr auto pixelsInMeter = 8.0f;

constexpr auto kickAngle = PI / 4;

constexpr auto gravityAcceleration = 9.8f;
constexpr auto airFriction = 0.03f * pixelsInMeter;
constexpr auto groundFriction = 10.0f * pixelsInMeter;

Texture generateShadow() {
  auto radius = Pig::pigWitdh / 2.0f;
  auto img = GenImageDither(radius * 2, radius * 2, BLACK, WHITE, 4);
  auto mask = GenImageColor(radius * 2, radius * 2, {0, 0, 0, 0});
  // TODO: ellipse
  ImageDrawCircle(&mask, radius, radius, radius, WHITE);
  ImageAlphaMask(&img, mask);
  auto res = LoadTextureFromImage(img);
  UnloadImage(img);
  UnloadImage(mask);
  return res;
}

Pig::Pig(hlam::Vec2 pos)
    : Entity(pos, pigWitdh, pigHeight),
      speed{0, 0},
      elevation(0),
      elevationSpeed_(0.0f),
      isKicked_(false),
      shadow_(generateShadow()) {}

void Pig::DoKick(Kick kick) {
  elevationSpeed_ = kick.impulse * sinf(kickAngle) * 10;  // fixme
  auto horizontalSpeed = kick.impulse * cosf(kickAngle);

  speed = kick.dir * horizontalSpeed;
  isKicked_ = true;
}

void Pig::Update(float dt) {
  if (isKicked_) {
    if (elevation >= 0.0f) {
      std::cout << elevation << std::endl;
      elevation += elevationSpeed_ * dt;
      elevationSpeed_ -= gravityAcceleration * dt;
    } else {
      elevationSpeed_ = 0.0f;
      elevation = 0.0f;
      isKicked_ = false;
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
    }
  }

  pos += speed;
}

void Pig::Draw() {
  if (elevation > 0) {
    DrawTextureV(shadow_, pos - drawDelta, WHITE);
  }

  DrawEllipse(pos.x, pos.y - elevation, width / 2, height / 2, BLACK);
  DrawEllipseLines(pos.x, pos.y - elevation, width / 2, height / 2, WHITE);
}
