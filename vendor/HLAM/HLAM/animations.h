#ifndef HLAM_ANIMATIONS_H
#define HLAM_ANIMATIONS_H

#include <vector>

namespace hlam {
#ifndef RAYLIB_H
struct Tex {
  unsigned int id;
  int width;
  int height;
  int mipmaps;
  int format;
};
#else
// use raylib's Texture in hlam namespace
struct Tex : public ::Texture {
  Tex(::Texture&& tex) : ::Texture(std::move(tex)) {}
};
#endif

class Animation {
 public:
  explicit Animation(std::vector<Tex>&& frames, float frameDuration, bool looping);
  explicit Animation(std::vector<Tex>&& frames, float frameDuration);

  void Update(float dt);

  void Reset();

  const Tex& GetFrame() const;

  std::vector<Tex> frames;
  float frameDuration;
  bool looping;
  bool finished;

 private:
  int currentFrameIdx;
  float time;
};
}  // namespace hlam

#endif  // HLAM_ANIMATIONS_H

#ifdef HLAM_ANIMATIONS_IMPLEMENTATION

namespace hlam {
inline Animation::Animation(std::vector<Tex>&& frames, float frameDuration, bool looping)
    : frames(std::move(frames)),
      frameDuration(frameDuration),
      looping(looping),
      finished(false),
      time(0),
      currentFrameIdx(0) {}

inline Animation::Animation(std::vector<Tex>&& frames, float frameDuration)
    : Animation(std::move(frames), frameDuration, false) {}

inline void Animation::Update(float dt) {
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

inline void Animation::Reset() {
  time = 0.0f;
  currentFrameIdx = 0;
  finished = false;
}

inline const Tex& Animation::GetFrame() const {
  // I prefer std::out_of_range() more than segfault
  return frames.at(currentFrameIdx);
}

}  // namespace hlam

#endif
