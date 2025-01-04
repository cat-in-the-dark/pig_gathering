#ifndef TEXTURES_H
#define TEXTURES_H

#include <raylib.h>
#include <stdlib.h>

Image GenImageDither(int width, int height, Color col1, Color col2, int fill_nth) {
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

#endif  // TEXTURES_H
