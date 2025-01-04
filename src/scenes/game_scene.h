#pragma once

#include <raylib.h>

// raylib must be before
#include <HLAM/hlam_math.h>
#include <HLAM/scene.h>

#include <memory>
#include <vector>

#include "entities/pig.h"
#include "entities/player.h"
#include "entities/truck.h"
#include "state.h"

class GameScene : public hlam::Scene {
  hlam::SceneManager* sm;
  GameState* gameState;
  std::vector<std::unique_ptr<Player>> players;
  std::vector<std::unique_ptr<Pig>> pigs;
  Truck truck;
  Camera2D camera;

  Texture2D grass;

 public:
  GameScene(hlam::SceneManager* sm, GameState* gameState);
  void Activate() override;
  void Update(float dt) override;
  void Draw() override;
  void Exit() override;
  ~GameScene();
};
