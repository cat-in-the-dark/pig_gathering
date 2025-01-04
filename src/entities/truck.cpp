#include "truck.h"

#include "const.h"

Truck::Truck(hlam::Vec2 pos, hlam::Vec2 size, hlam::Vec2 speed) : pos(pos), size(size), speed(speed) {}

void Truck::Update(float dt) {
  // TODO: add jitter and some effects
  pos += speed * dt;
}

void Truck::Draw() {
  DrawRectangleV(pos, size, BROWN);
}
