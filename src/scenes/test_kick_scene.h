#ifndef TEST_KICK_SCENE_H
#define TEST_KICK_SCENE_H

#include <HLAM/hlam_math.h>

#undef HLAM_SCENE_IMPLEMENTATION
#include <HLAM/scene.h>

#include <vector>

class Pig;

class TestKickScene : public hlam::Scene {
 public:
  void Activate() override;
  void Exit() override;
  void Update(float dt) override;
  void Draw() override;

 private:
  std::vector<std::shared_ptr<Pig>> pigs;
  hlam::Vec2 kickDir;
  float kickPower;
};

#endif  // TEST_KICK_SCENE_H
