#pragma once

#include <HLAM/scene.h>

#include "state.h"

class ResultsScene : public hlam::Scene {
  GameState* state;

 public:
  ResultsScene(GameState* state);
  void Activate() override;
  void Update(float dt) override;
  void Draw() override;
  void Exit() override;
  ~ResultsScene();
};
