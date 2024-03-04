#pragma once
#include <cstdlib>
#include <cstddef>

extern "C" {
    #include <linux/fb.h>
    #include <fcntl.h> 
    #include <sys/ioctl.h>
    #include <sys/mman.h>
    #include <inttypes.h>
    #include <stdint.h>
    #include <unistd.h>
}

void drawpixel(uint32_t x, uint32_t y, const char *rgb);

void drawrect(int x, int y, int x1, int y1, const char *rgb);

int fb_init();
