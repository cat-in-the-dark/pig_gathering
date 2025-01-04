#pragma once

#include <raylib.h>
// raylib must be before
#include <HLAM/hlam_math.h>
#include <HLAM/scene.h>

#include "entities/player.h"
#include "entities/truck.h"

class ControlsScene : public hlam::Scene {
  hlam::SceneManager* sm;
  Player player;
  Truck truck;
  Camera2D camera;

  Texture2D grass;

 public:
  ControlsScene(hlam::SceneManager* sm);
  void Activate() override;
  void Update(float dt) override;
  void Draw() override;
  void Exit() override;
  ~ControlsScene();
};
