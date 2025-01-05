#include "wolf.h"

#include <HLAM/hlam_math.h>

#include "assets.h"
#include "const.h"
#include "helpers/texture_helper.h"
#include "pig.h"

WolfBehaviour::~WolfBehaviour() = default;
void WolfBehaviour::Activate() {}
void WolfBehaviour::Exit() {}

KidnapWolfBehaviour::KidnapWolfBehaviour(Wolf* wolf) : wolf(wolf), target({kWorldPosRight + 64, kWorldPosDown}) {}
void KidnapWolfBehaviour::Activate() {
  wolf->currentAnim = &(wolf->walkAnim);
}
void KidnapWolfBehaviour::Update(float dt) {
  // stupid wolf ALWAYS run to the same place
  auto dir = target - wolf->pos;
  auto move = hlam::vec_norm(dir) * (wolf->speed * dt);
  wolf->pos += move;
  wolf->dirX = dir.x > 0 ? 1 : -1;
  if (wolf->closestPig != nullptr && wolf->closestPig->GetState() == Pig::State::KIDNAPPED) {
    if (hlam::vec_dist(wolf->pos, wolf->closestPig->pos) > wolf->size.x / 2 + wolf->closestPig->size.x / 2) {
      wolf->closestPig->Release();
    } else {
      wolf->closestPig->pos += move;
    }
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
  wolf->currentAnim = &(wolf->idleAnim);
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
void ChaseWolfBehaviour::Activate() {
  wolf->currentAnim = &(wolf->chaseAnim);
}
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
  wolf->dirX = dir.x > 0 ? 1 : -1;
}

WanderWolfBehaviour::WanderWolfBehaviour(Wolf* wolf) : wolf(wolf) {}
void WanderWolfBehaviour::Activate() {
  wolf->currentAnim = &(wolf->walkAnim);
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
    wolf->dirX = dir.x > 0 ? 1 : -1;
    // TODO: it should be more presice
    wolf->pos += hlam::vec_norm(dir) * (wolf->speed * dt);
  }
}

KickedWolfBehaviour::KickedWolfBehaviour(Wolf* wolf) : wolf(wolf) {}

void KickedWolfBehaviour::Activate() {
  wolf->currentAnim = &(wolf->idleAnim);
  PlaySound(wolfKickFX);
}
void KickedWolfBehaviour::Update(float dt) {
  auto timeScale = dt * 3;

  auto& st = wolf->kickState;
  st.elevation += st.elevationSpeed * timeScale;
  st.elevationSpeed -= physics::gravityAcceleration * timeScale;

  // FIXME: this was added to balance out previous bug. Please fix it along with dt * 3
  wolf->pos += st.kickSpeed;

  if (st.elevation < 0.0f) {
    wolf->ChangeState(IDLE);
    st = {};
  }
}

Wolf::Wolf(hlam::Vec2 pos, hlam::Vec2 size, float speed, float runSpeed)
    : behaviours(),
      shadow_(generateShadow(size.x)),
      pos(pos),
      size(size),
      speed(speed),
      runSpeed(runSpeed),
      closestPig(nullptr),
      idleAnim(wolfIdleFrames, 0.5f, true),
      walkAnim(wolfWalkFrames, 0.5f, true),
      chaseAnim(wolfWalkFrames, 0.2f, true),
      currentAnim(&idleAnim) {
  behaviours.emplace(IDLE, std::make_unique<IdleWolfBehaviour>(this));
  behaviours.emplace(CHASE, std::make_unique<ChaseWolfBehaviour>(this));
  behaviours.emplace(WANDER, std::make_unique<WanderWolfBehaviour>(this));
  behaviours.emplace(KIDNAPPING, std::make_unique<KidnapWolfBehaviour>(this));
  behaviours.emplace(KICKED, std::make_unique<KickedWolfBehaviour>(this));
}

Wolf::~Wolf() {
  UnloadTexture(shadow_);
}

void Wolf::ChangeState(WolfState stateToChange) {
  nextState = stateToChange;
}

void Wolf::DoKick(Kick kick) {
  kickState.elevationSpeed = kick.impulse * sinf(balance::kKickAngle);
  auto horizontalSpeed = kick.impulse * cosf(balance::kKickAngle);

  kickState.kickSpeed = kick.dir * horizontalSpeed;
  ChangeState(KICKED);
}

void Wolf::Update(float dt) {
  currentAnim->Update(dt);
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
    if (state != KICKED) {
      die_ = true;
    }

    if (state == KIDNAPPING) {
      if (closestPig != nullptr) {
        closestPig->isDead = true;
      }
    }
  }
}

void Wolf::Draw() {
  // FIXME: fix speed increment and remove elevation multiplier
  auto drawPos = pos;
  if (state == KICKED) {
    hlam::Vec2 drawDelta = size / 2.0f;
    auto shadowYOffset = hlam::Vec2{0.0f, -size.x / 2};

    hlam::Vec2 elevatedDelta = {0, -kickState.elevation * 50};
    drawPos += elevatedDelta;

    DrawTextureV(shadow_, pos - drawDelta + shadowYOffset, WHITE);
  }

  // DrawCircleLinesV(drawPos, (size.x + size.y) / 2, RED);
  auto frame = currentAnim->GetFrame();
  Rectangle src = {0, 0, static_cast<float>(frame.width) * dirX, static_cast<float>(frame.height)};
  Rectangle dst = {drawPos.x, drawPos.y, static_cast<float>(frame.width), static_cast<float>(frame.height)};
  DrawTexturePro(frame, src, dst, Vector2{static_cast<float>(frame.width) / 2, static_cast<float>(frame.height) / 2}, 0,
                 WHITE);
}
