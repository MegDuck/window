#ifndef DRAW_H
#define DRAW_H
#include <stdint.h>
int read_fb(char *device);

//draw a pixel on x and y
//with color #rgb
void drawpixel(uint32_t x, uint32_t y, const char *rgb);

void drawrect(int x, int y, int x1, int y1, const char *rgb);

int fb_init();
#endif
