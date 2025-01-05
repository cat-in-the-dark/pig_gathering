#include "wolf.h"

#include "const.h"
#include "pig.h"

WolfBehaviour::~WolfBehaviour() = default;
void WolfBehaviour::Activate() {}
void WolfBehaviour::Exit() {}

KidnapWolfBehaviour::KidnapWolfBehaviour(Wolf* wolf) : wolf(wolf), target({kWorldPosRight + 64, kWorldPosDown}) {}
void KidnapWolfBehaviour::Update(float dt) {
  // stupid wolf ALWAYS run to the same place
  auto dir = target - wolf->pos;
  auto move = hlam::vec_norm(dir) * (wolf->speed * dt);
  wolf->pos += move;
  if (wolf->closestPig != nullptr && wolf->closestPig->GetState() == Pig::State::KIDNAPPED) {
    wolf->closestPig->pos += move;
  } else {
    wolf->ChangeState(IDLE);
  }
}

IdleWolfBehaviour::IdleWolfBehaviour(Wolf* wolf)
    : wolf(wolf), timer(static_cast<float>(GetRandomValue(100, 300)) / 100) {}
void IdleWolfBehaviour::Activate() {
  // Play Awooo
  // todo: maybe some random time jitter?
  timer.time = static_cast<float>(GetRandomValue(100, 300)) / 100;
  timer.Reset();
}
void IdleWolfBehaviour::Update(float dt) {
  timer.Update(dt);

  // TODO: check closest pig and change state immediately
  if (timer.IsPassed()) {
    // TODO: if pig is close enough -> Chase

    if (wolf->closestPig != nullptr) {
      wolf->ChangeState(CHASE);
    } else {
      wolf->ChangeState(WANDER);
    }
  }
}

ChaseWolfBehaviour::ChaseWolfBehaviour(Wolf* wolf) : wolf(wolf) {}
void ChaseWolfBehaviour::Update(float dt) {
  if (wolf->closestPig == nullptr) {
    wolf->ChangeState(IDLE);
    return;
  }

  auto dir = wolf->closestPig->pos - wolf->pos;
  if (hlam::vec_length(dir) < 4) {
    wolf->closestPig->Kidnapped();
    wolf->ChangeState(KIDNAPPING);
    return;
  }

  // TODO: maybe tired???
  wolf->pos += hlam::vec_norm(dir) * (wolf->runSpeed * dt);
}

WanderWolfBehaviour::WanderWolfBehaviour(Wolf* wolf) : wolf(wolf) {}
void WanderWolfBehaviour::Activate() {
  // random point around
  float radius = GetRandomValue(kMinWolfWanderDist, kMaxWolfWanderDist);
  float angle = GetRandomValue(0, 360);
  hlam::Vec2 dir{cosf(angle), sinf(angle)};
  target = wolf->pos + dir * radius;
  // do not walk outside the world
  target.x = std::clamp(target.x, kWorldPosLeft, kWorldPosRight);
  target.y = std::clamp(target.y, kWorldPosUp, kWorldPosDown);
}
void WanderWolfBehaviour::Update(float dt) {
  // TODO: check closest pig and change state immediately
  hlam::Vec2 dir = target - wolf->pos;
  TraceLog(LOG_INFO, "%f", hlam::vec_length(dir));
  if (hlam::vec_length(dir) < 2) {  // at least 2 pixels...
    if (wolf->closestPig != nullptr) {
      wolf->ChangeState(CHASE);
    } else {
      wolf->ChangeState(WANDER);
    }
  } else {
    // TODO: it should be more presice
    wolf->pos += hlam::vec_norm(dir) * (wolf->speed * dt);
  }
}

Wolf::Wolf(hlam::Vec2 pos, hlam::Vec2 size, float speed, float runSpeed)
    : behaviours(), pos(pos), size(size), speed(speed), runSpeed(runSpeed), closestPig(nullptr) {
  behaviours.emplace(IDLE, std::make_unique<IdleWolfBehaviour>(this));
  behaviours.emplace(CHASE, std::make_unique<ChaseWolfBehaviour>(this));
  behaviours.emplace(WANDER, std::make_unique<WanderWolfBehaviour>(this));
  behaviours.emplace(KIDNAPPING, std::make_unique<KidnapWolfBehaviour>(this));
}

void Wolf::ChangeState(WolfState stateToChange) {
  nextState = stateToChange;
}

void Wolf::Update(float dt) {
  if (closestPig != nullptr && closestPig->isDead) {
    closestPig = nullptr;
  }

  if (state != nextState) {
    TraceLog(LOG_INFO, "Change Wolf() state from %d to %d", state, nextState);
    if (behaviours.count(state) != 0) {
      behaviours.at(state)->Exit();
    }
    if (behaviours.count(nextState) != 0) {
      behaviours.at(nextState)->Activate();
    }
    state = nextState;
  }

  if (behaviours.count(state) != 0) {
    behaviours.at(state)->Update(dt);
  }

  if (pos.x >= kWorldPosRight || pos.y >= kWorldPosDown || pos.y <= kWorldPosUp || pos.x <= kWorldPosLeft) {
    die_ = true;
    if (closestPig != nullptr) {
      closestPig->isDead = true;
    }
  }
}

void Wolf::Draw() {
  DrawCircleV(pos, (size.x + size.y) / 2, RED);
}
