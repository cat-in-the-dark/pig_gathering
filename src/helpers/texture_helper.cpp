#include "texture_helper.h"

#include <HLAM/textures.h>

Texture generateShadow(float radius) {
  auto img = GenImageDither(radius * 2, radius * 2, BLACK, WHITE, 4);
  auto mask = GenImageColor(radius * 2, radius * 4, {0, 0, 0, 0});
  // TODO: ellipse
  ImageDrawCircle(&mask, radius, radius, radius, WHITE);
  ImageResizeNN(&mask, radius * 2, radius * 2);
  ImageAlphaMask(&img, mask);
  ClearAlphaColor(img, BLACK);
  auto res = LoadTextureFromImage(img);
  UnloadImage(img);
  UnloadImage(mask);
  return res;
}
