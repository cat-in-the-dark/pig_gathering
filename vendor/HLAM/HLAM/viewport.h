#ifndef HLAM_VIEWPORT_H
#define HLAM_VIEWPORT_H

namespace hlam {

class Viewport {
  // calculated cache
  float scale;
  Rectangle canvas_rect;
  Rectangle screen_rect;

 public:
  int canvas_width;
  int canvas_height;
  Vector2 origin;
  // int mode; // TODO: enum viewport mode: pixel perfect, just fit, etc...

  Viewport(int canvas_width, int canvas_height, Vector2 origin);
  void update();
  void draw(const Texture& render_texture, const Color& color) const;
};

}  // namespace hlam

#endif  // HLAM_VIEWPORT_H

#ifdef HLAM_VIEWPORT_IMPLEMENTATION

#include <cmath>

namespace hlam {

Viewport::Viewport(int canvas_width, int canvas_height, Vector2 origin)
    : canvas_width(canvas_width), canvas_height(canvas_height), origin(origin) {}
void Viewport::update() {
  float screen_width = GetScreenWidth();
  float screen_height = GetScreenHeight();
  scale = std::fmin(screen_width / canvas_width, screen_height / canvas_height);
  screen_rect = {
      (screen_width - canvas_width * scale) * 0.5f,
      (screen_height - canvas_height * scale) * 0.5f,
      canvas_width * scale,
      canvas_height * scale,
  };
  canvas_rect = {
      0,
      0,
      static_cast<float>(canvas_width),
      static_cast<float>(-canvas_height),
  };
}

void Viewport::draw(const Texture& render_texture, const Color& color = WHITE) const {
  DrawTexturePro(render_texture, canvas_rect, screen_rect, origin, 0.0f, color);
}

}  // namespace hlam

#endif  // HLAM_VIEWPORT_IMPLEMENTATION