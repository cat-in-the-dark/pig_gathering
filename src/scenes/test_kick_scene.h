#ifndef TEST_KICK_SCENE_H
#define TEST_KICK_SCENE_H

#undef HLAM_SCENE_IMPLEMENTATION
#include <HLAM/hlam_math.h>
#include <HLAM/scene.h>

#include <vector>

class Entity;

class TestKickScene : public hlam::Scene {
 public:
  void Activate() override;
  void Exit() override;
  void Update(float dt) override;
  void Draw() override;

 private:
  std::vector<std::shared_ptr<Entity>> entities;
  hlam::Vec2 kickDir;
  float kickPower;
};

#endif  // TEST_KICK_SCENE_H
