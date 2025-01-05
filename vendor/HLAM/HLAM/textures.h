#ifndef TEXTURES_H
#define TEXTURES_H

#include <raylib.h>
#include <stdlib.h>

inline bool operator==(const Color &c1, const Color &c2) {
  return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}

inline Image GenImageDither(int width, int height, Color col1, Color col2, int fill_nth) {
  Color *pixels = (Color *) RL_MALLOC(width * height * sizeof(Color));

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if ((y % 2 == 0) || ((x + y) % fill_nth != 0)) {
        pixels[y * width + x] = col1;
      } else {
        pixels[y * width + x] = col2;
      }
    }
  }

  return {pixels, width, height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
}

inline void ClearAlphaColor(Image &image, Color color) {
  if (image.format == PIXELFORMAT_UNCOMPRESSED_R8G8B8A8) {
    auto *pixels = reinterpret_cast<Color *>(image.data);
    for (int y = 0; y < image.width; y++) {
      for (int x = 0; x < image.height; x++) {
        auto &imgColor = pixels[y * image.width + x];
        if (imgColor == color) {
          imgColor.a = 0;
        }
      }
    }
  }
}

#endif  // TEXTURES_H
