#include "draw.h"

//vars

struct fb_fix_screeninfo finfo_t;
struct fb_var_screeninfo vinfo_t;
size_t size;
uint8_t *fbp;

void drawpixel(uint32_t x, uint32_t y, const char *rgb);
void drawrect(int x, int y, int x1, int y1, const char *rgb);
uint64_t hextorgb(const char *str);


int fb_init() {
    int fd = open("/dev/fb0", O_RDWR);

    ioctl(fd, FBIOGET_VSCREENINFO, &vinfo_t);
    vinfo_t.grayscale=0;
    vinfo_t.bits_per_pixel=24;

    ioctl(fd, FBIOPUT_VSCREENINFO, &vinfo_t);
    ioctl(fd, FBIOGET_FSCREENINFO, &finfo_t);
    size = vinfo_t.yres * finfo_t.line_length;
    fbp = (uint8_t*) mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
}

/* int main(){
	//reading framebuffer 
	int fd = read_fb("");
	size = vinfo.yres * finfo.line_length;
    printf("%d", finfo.line_length);
	uint32_t x = 0;
	uint32_t y = 400;
	drawrect(0,0,300,300,"#C73434",1,0);
	return 0;
} */

void drawpixel(uint32_t x, uint32_t y, const char *rgb) {
	int pixel = hextorgb(rgb);
    
	uint32_t location = x*vinfo_t.bits_per_pixel/8 + y*finfo_t.line_length;
	*((uint32_t*) (fbp + location)) = pixel;
}

void drawrect(int x, int y, int x1, int y1, const char *rgb){
	for ( int i_x = x; i_x <= x1; i_x++ ) {
		for ( int i_y = y; i_y <= y1; i_y++ ) {
            drawpixel(i_x, i_y, rgb);
		}
	}
}
void draw_horizontal_line(int x1, int x2, int y, const char *rgb)
{
    for (int i = x1; i < x2; i++)
        drawpixel(i, y, rgb);
}

void draw_vertical_line(int x, int y1, int y2, const char *rgb)
{
    for (int i = y1; i < y2; i++) {
        drawpixel(x, i, rgb);
    }
}

void plot4points(double cx, double cy, double x, double y, const char *rgb)
{
    drawpixel(cx + x, cy + y, rgb);
    drawpixel(cx - x, cy + y, rgb);
    drawpixel(cx + x, cy - y, rgb);
    drawpixel(cx - x, cy - y, rgb);
}

void plot8points(double cx, double cy, double x, double y, const char *rgb)
{
    plot4points(cx, cy, x, y, rgb);
    plot4points(cx, cy, y, x, rgb);
}

void draw_circle(double cx, double cy, int radius, const char *rgb)
{
    int error = -radius;
    double x = radius;
    double y = 0;

    while (x >= y) {
        plot8points(cx, cy, x, y, rgb);

        error += y;
        y++;
        error += y;

        if (error >= 0)
        {
            error += -x;
            x--;
            error += -x;
        }
    }
}


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
