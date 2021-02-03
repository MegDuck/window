#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <fcntl.h> 
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <inttypes.h>
#include "util.h"
#include "window.h"
#include "../fbapi/draw.h"

//display ids
int display[500];
//vars
struct fb_fix_screeninfo finfo;
struct fb_var_screeninfo vinfo;

/*declarations of functions*/
//read_fb open /dev/fb0 or other device to read & write.
int read_fb(char *device);

void drawrect(int x, int y, int x1, int y1, uint32_t r, uint32_t g, uint32_t b, uint32_t a, int border);

//give a start values for window
void initwin(win *Win, int x, int y, int x1, int y1);

void display_win(win *Win);

void delete_win(win *Win);
//function for managing windows on struct {display}

int main(void){
	int fd = read_fb("");
	ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);
	ioctl(fd, FBIOGET_FSCREENINFO, &finfo);
	size_t size = vinfo.yres * finfo.line_length;
	uint8_t *fbp = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd,0);
	//initializing
	struct win *window = malloc(sizeof *window);
	init_win(window, 100, 100);
	window->name = "win_name";

	struct win *window1 = malloc(sizeof *window1);
	init_win(window1, 100, 100);
	window1->name = "win1_name";

	printf("\n%s->%d", window->name, window->id);
	printf("\n%s->%d", window1->name, window1->id);

	free(window);
	free(window1);
//	delwin(window);
	return 0;
}


//managing windows
void init_win(struct win *Win, int x, int y, int x1, int y1){
	printf("start of initwin\n");
	int wid=0;
	while (1){
		//window id
		printf("\n Window get id...");
		wid = rand()%sizeof(display) / sizeof(display[0]);
		int found = 1;
		for (int id=0; id<sizeof(display) / sizeof(display[0]) * 2; id++) {
			if (id == wid){
				int found=0;
			}
		}
		if (found == 1){
			printf("get id for window!\n");
			break;
		} else {
			printf("invalid id.. serching any other..\n");
		}
	}
	Win->id = wid;
	Win->x = 200;
	Win->y = 200;
	Win->x1 = 500;
	Win->y1 = 600;
	display[sizeof(display) / sizeof(display[0])+1] = wid;
}

void display_win(win *Win, 0x00000){
	drawrect(Win->x, Win->y, Win->x1, Win->y1)
}
void delete_win(){
	/* del window from id stack and from display and free memory*/
}
/*Creator's dialog-----*/
// -how are you?       //
// -I'm good. And you? //
// -I'm fine. Thanks.  //
/*---------------------*/


