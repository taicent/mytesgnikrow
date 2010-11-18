#ifndef GifIncluded
#define GifIncluded

#include "img.hpp"

WidthHeight *ReadGifSize(char *filename);
void LoadByteImageGif(ByteImage *image, char *file);

extern "C" {
unsigned char *LoadGifToBuffer(char *filename, int *width, int *height);
}

#endif
