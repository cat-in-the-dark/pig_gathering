#include "controls_scene.h"

#include <algorithm>

#include "const.h"

using namespace hlam;

ControlsScene::ControlsScene(SceneManager* sm)
    : sm(sm),
      player(0, Vec2{kPlayerSpawnPosX, kPlayerSpawnPosY}, kPlayerSpeed),
      truck(Vec2{kTruckPosX, kTruckPosY}, {64, 32}, {kTruckSpeedX, kTruckSpeedY}) {}
void ControlsScene::Activate() {
  camera.offset = {kWindowHeight / 2, kWindowHeight / 2};
  camera.target = {0, 0};
  camera.rotation = 0;
  camera.zoom = 1;
}
void ControlsScene::Update(float dt) {
  truck.Update(dt);
  player.Update(dt);

  camera.target.x = std::clamp(player.pos.x, kWorldPosLeft + camera.offset.x, kWorldPosRight - camera.offset.x);
  camera.target.y = std::clamp(player.pos.y, kWorldPosUp + camera.offset.y, kWorldPosDown - camera.offset.y);

  if (truck.pos.x >= kWindowWidth) {
    sm->Change("results");
  }
}
void ControlsScene::Draw() {
  BeginMode2D(camera);
  DrawRectangleLines(kPlayerPosLeft + 2, kPlayerPosUp + 2, kPlayerPosRight - 4, kPlayerPosDown - 4, RED);
  truck.Draw();
  player.Draw();
  EndMode2D();
}
void ControlsScene::Exit() {}
