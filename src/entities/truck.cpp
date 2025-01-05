#include "truck.h"

#include "const.h"

Truck::Truck(hlam::Vec2 pos, hlam::Vec2 size, hlam::Vec2 speed) : pos(pos), size(size), speed(speed) {}

void Truck::Update(float dt) {
  // TODO: add jitter and some effects
  pos += speed * dt;
}

void Truck::Draw() {
  DrawRectangleV(pos, size, BROWN);
  auto hopperRect = GetHopperRect();
  DrawRectangleRec(hopperRect, LIGHTGRAY);
}

hlam::Rect Truck::GetHopperRect() const {
  auto centerPos = pos + size / 2;
  auto hopperTopleft = centerPos - Truck::kHopperSize / 2 + Truck::kHopperOffset;
  return hlam::Rect{hopperTopleft.x, hopperTopleft.y, Truck::kHopperSize.x, Truck::kHopperSize.y};
}
