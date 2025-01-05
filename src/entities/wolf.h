#pragma once

#include <HLAM/animations.h>
#include <HLAM/coroutines.h>
#include <HLAM/hlam_math.h>

#include <memory>
#include <unordered_map>

#include "kick.h"

enum WolfState { IDLE, WANDER, CHASE, KIDNAPPING, KICKED };

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
  void Activate() override;
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
  void Activate() override;
  void Update(float dt) override;
};

class KickedWolfBehaviour : public WolfBehaviour {
  Wolf* wolf;

 public:
  KickedWolfBehaviour(Wolf* wolf);
  void Activate() override;
  void Update(float dt) override;
};

struct KickState {
  float elevation;
  float elevationSpeed;
  hlam::Vec2 kickSpeed;
};

class Pig;
class Wolf {
  WolfState nextState = IDLE;
  WolfState state = IDLE;
  std::unordered_map<WolfState, std::unique_ptr<WolfBehaviour>> behaviours;
  bool die_ = false;
  Texture shadow_;

 public:
  hlam::Vec2 pos;
  hlam::Vec2 size;
  float speed;
  float runSpeed;
  KickState kickState{};

  Pig* closestPig;
  float dirX = 1;
  hlam::Animation idleAnim;
  hlam::Animation walkAnim;
  hlam::Animation chaseAnim;
  hlam::Animation* currentAnim;

  Wolf(hlam::Vec2 pos, hlam::Vec2 size, float speed, float runSpeed);
  virtual ~Wolf();
  void Update(float dt);
  void Draw();
  WolfState GetState() const {
    return state;
  }
  void DoKick(Kick kick);
  void ChangeState(WolfState state);
  bool IsDead() const {
    return die_;
  }
};
