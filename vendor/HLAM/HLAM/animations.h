#ifndef HLAM_ANIMATIONS_H
#define HLAM_ANIMATIONS_H

#include <raylib.h>

#include <vector>

namespace hlam {

class Animation {
 public:
  explicit Animation(std::vector<Texture> frames, float frameDuration, bool looping);
  explicit Animation(std::vector<Texture> frames, float frameDuration);

  void Update(float dt);

  void Reset();

  const Texture& GetFrame() const;

  std::vector<Texture> frames;
  float frameDuration;
  bool looping;
  bool finished;

 private:
  float time;
  std::size_t currentFrameIdx;
};
}  // namespace hlam

#endif  // HLAM_ANIMATIONS_H

#ifdef HLAM_ANIMATIONS_IMPLEMENTATION

namespace hlam {
Animation::Animation(std::vector<Texture> frames, float frameDuration, bool looping)
    : frames(frames), frameDuration(frameDuration), looping(looping), finished(false), time(0), currentFrameIdx(0) {}

Animation::Animation(std::vector<Texture> frames, float frameDuration) : Animation(frames, frameDuration, false) {}

void Animation::Update(float dt) {
  if (finished) {
    return;
  }

  time += dt;
  if (time >= frameDuration) {
    time = 0;
    currentFrameIdx++;
    if (currentFrameIdx >= frames.size()) {
      if (looping) {
        currentFrameIdx = 0;
      } else {
        currentFrameIdx = frames.size() - 1;
        finished = true;
      }
    }
  }
}

void Animation::Reset() {
  time = 0.0f;
  currentFrameIdx = 0;
  finished = false;
}

const Texture& Animation::GetFrame() const {
  // I prefer std::out_of_range() more than segfault
  return frames.at(currentFrameIdx);
}

}  // namespace hlam

#endif
