#pragma once

#include <HLAM/coroutines.h>
#include <HLAM/hlam_math.h>

#include <memory>
#include <unordered_map>

enum WolfState { IDLE, WANDER, CHASE, KIDNAPPING };

class WolfBehaviour {
 public:
  virtual void Update(float dt) = 0;
  virtual void Activate();
  virtual void Exit();
  virtual ~WolfBehaviour();
};

class Wolf;
class IdleWolfBehaviour : public WolfBehaviour {
  Wolf* wolf;
  hlam::Timer timer;

 public:
  IdleWolfBehaviour(Wolf* wolf);
  void Update(float dt) override;
  void Activate() override;
};

class ChaseWolfBehaviour : public WolfBehaviour {
  Wolf* wolf;

 public:
  ChaseWolfBehaviour(Wolf* wolf);
  void Update(float dt) override;
};

class WanderWolfBehaviour : public WolfBehaviour {
  Wolf* wolf;
  hlam::Vec2 target;

 public:
  WanderWolfBehaviour(Wolf* wolf);
  void Update(float dt) override;
  void Activate() override;
};

class KidnapWolfBehaviour : public WolfBehaviour {
  Wolf* wolf;
  hlam::Vec2 target;

 public:
  KidnapWolfBehaviour(Wolf* wolf);
  void Update(float dt) override;
};

class Pig;
class Wolf {
  WolfState nextState = IDLE;
  WolfState state = IDLE;
  std::unordered_map<WolfState, std::unique_ptr<WolfBehaviour>> behaviours;
  bool die_ = false;

 public:
  hlam::Vec2 pos;
  hlam::Vec2 size;
  float speed;
  float runSpeed;

  Pig* closestPig;

  Wolf(hlam::Vec2 pos, hlam::Vec2 size, float speed, float runSpeed);
  void Update(float dt);
  void Draw();
  void ChangeState(WolfState state);
  bool IsDead() const {
    return die_;
  }
};
