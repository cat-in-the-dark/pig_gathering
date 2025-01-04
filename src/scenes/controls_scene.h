#pragma once

#include <HLAM/scene.h>

class ControlsScene : public hlam::Scene {
 public:
  ControlsScene();
  void Activate() override;
  void Update(float dt) override;
  void Draw() override;
  void Exit() override;
};
