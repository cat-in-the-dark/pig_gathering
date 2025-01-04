#pragma once

#include <raylib.h>
// raylib must be before
#include <HLAM/hlam_math.h>
#include <HLAM/scene.h>

#include "entities/player.h"

class ControlsScene : public hlam::Scene {
  Player player;
  Camera2D camera;

 public:
  ControlsScene();
  void Activate() override;
  void Update(float dt) override;
  void Draw() override;
  void Exit() override;
};
