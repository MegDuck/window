#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <fcntl.h> 
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <inttypes.h>
#include "util.h" 
//vars

struct fb_fix_screeninfo finfo;
struct fb_var_screeninfo vinfo;
size_t size;
uint8_t *fbp;
//declarations of functions

//read_fb open /dev/fb0 or other device to read & write.
int read_fb(char *device);

//draw a pixel on x and y
//with color #rgb
void drawpixel(uint32_t x, uint32_t y, const char *rgb);

void drawrect(int x, int y, int x1, int y1, const char *rgb, int border, int border_size);

uint64_t hextorgb(const char *str);
//main: for debug and testing

int main(){
	//reading framebuffer 
	//note: fd - is FrameBuffer
	int fd = read_fb("");
	ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);
	ioctl(fd, FBIOGET_FSCREENINFO, &finfo);
	size = vinfo.yres * finfo.line_length;
	fbp = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	uint32_t x = 0;
	uint32_t y = 400;
	drawrect(0,0,300,300,"#C73434",1,0);
	return 0;
}
void drawpixel(uint32_t x, uint32_t y, const char *rgb) {
	int pixel = hextorgb(rgb);
	printf("%d", pixel);
	uint32_t location = x*vinfo.bits_per_pixel/8 + y*finfo.line_length;
	*((uint32_t*) (fbp + location)) = pixel;
}

void drawrect(int x, int y, int x2, int y2, const char *rgb, int border, int border_size){
	for ( int i_x=x; i_x<x2; i_x++){
		for ( int i_y=y; i_y<y2; i_y++){
			if (border == 1){
				if (i_y <= y || i_y >= y2 || i_x >= x || i_x == x2){
					//draw a border with -rgba color
					drawpixel(i_x, i_y, rgb);
				} else {
					drawpixel(i_x, i_y, rgb);
				}
			} else {
				//draw a pixel on i_x and on i_y with rgba color.
				drawpixel(i_x, i_y, rgb);
			}
		}
	}
}
//void drawline(int y,int x)


//convert HEX to rgb64
uint64_t hextorgb(const char *str)
{
    uint64_t res = 0;
    char c;

    while ((c = *str++)) {
        char v = (c & 0xF) + (c >> 6) | ((c >> 3) & 0x8);
        res = (res << 4) | (uint64_t) v;
    }

    return res;
}
