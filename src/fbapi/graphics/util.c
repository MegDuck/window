#include <fcntl.h>

int read_fb(char *device){
	// /dev/fb{0,1} or /dev/graphics/fb{0,1}
	if ( device == "") {
		device = "/dev/fb0";
	}

	int fd = open("/dev/fb0", O_RDWR);
	return fd;
}
