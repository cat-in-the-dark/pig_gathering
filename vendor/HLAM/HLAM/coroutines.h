#ifndef HLAM_COROUTINES_H
#define HLAM_COROUTINES_H

namespace hlam {
// Timer checks whether time is passed or not.
class Timer {
  const float time;
  float elapsed;

 public:
  Timer(float time);

  bool IsPassed() const;
  void Update(float dt);
  void Reset();
  float Elapsed() const;
};

// OneShot checks whether time is passed or not and returns true only once
class OneShot {
  const float time;
  float elapsed;
  bool passed = false;

 public:
  OneShot(float time);

  bool IsPassed();
  void Update(float dt);
  void Reset();
};

// Cooldown is a Timer with auto reset after passing
class Cooldown {
  const float time;
  float elapsed;

 public:
  Cooldown(float time, float init);
  Cooldown(float time);

  bool Invoke();
  void Update(float dt);
  void Reset();
};
}  // namespace hlam

#endif  // HLAM_COROUTINES_H

#ifdef HLAM_COROUTINES_IMPLEMENTATION

namespace hlam {
Timer::Timer(float time) : time(time), elapsed(0) {}
bool Timer::IsPassed() const { return elapsed >= time; }
void Timer::Update(float dt) { elapsed += dt; }
void Timer::Reset() { elapsed = 0; }
float Timer::Elapsed() const { return elapsed; }

OneShot::OneShot(float time) : time(time), elapsed(0.0f) {}
bool OneShot::IsPassed() {
  if (passed) {
    return false;
  }
  if (elapsed >= time) {
    passed = true;
    return true;
  }
  return false;
}
void OneShot::Update(float dt) { elapsed += dt; }
void OneShot::Reset() {
  elapsed = 0.0f;
  passed = false;
}

Cooldown::Cooldown(float time) : time(time), elapsed(0) {}
Cooldown::Cooldown(float time, float init) : time(time), elapsed(init) {}
bool Cooldown::Invoke() {
  if (elapsed >= time) {
    elapsed = 0;
    return true;
  }
  return false;
}
void Cooldown::Update(float dt) { elapsed += dt; }
void Cooldown::Reset() { elapsed = 0; }
}  // namespace hlam

#endif  // HLAM_COROUTINES_IMPLEMENTATION