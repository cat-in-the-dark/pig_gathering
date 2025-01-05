#include "truck.h"

#include "assets.h"
#include "const.h"

Truck::Truck(hlam::Vec2 pos, hlam::Vec2 size, hlam::Vec2 speed)
    : pos(pos), size(size), speed(speed), anim(truckTex, 0.5, true) {}

void Truck::Update(float dt) {
  // TODO: add jitter and some effects
  pos += speed * dt;
  anim.Update(dt);
}

void Truck::Draw() {
  // DrawRectangleV(pos, size, BROWN);
  // auto hopperRect = GetHopperRect();
  // DrawRectangleRec(hopperRect, LIGHTGRAY);
  hlam::Vec2 texSize{0, static_cast<float>(truckTex[0].height)};
  DrawTextureEx(anim.GetFrame(), pos - texSize / 2, 0, 2, WHITE);
}

hlam::Rect Truck::GetHopperRect() const {
  auto centerPos = pos + size / 2;
  auto hopperTopleft = centerPos - Truck::kHopperSize / 2 + Truck::kHopperOffset;
  return hlam::Rect{hopperTopleft.x, hopperTopleft.y, Truck::kHopperSize.x, Truck::kHopperSize.y};
}
