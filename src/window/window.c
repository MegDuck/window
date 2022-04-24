#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>
#include<stdlib.h>
#include <linux/input-event-codes.h>
#include <linux/fb.h>
#include "../fbapi/event.h"
#include "../fbapi/graphics/util.h"
#include "../fbapi/graphics/draw.h"
#include "window-codes.h"
#include <pthread.h>
#include <termios.h>
//vars
struct fb_fix_screeninfo finfo;
struct fb_var_screeninfo vinfo;



int enabled = 0;
/*declarations of functions*/
//read_fb open /dev/fb0 or other device to read & write.
int read_fb(char *device);
int fb_init();
struct client_event read_client();
void thread_f(void);
void client_thread(void);
void send_data_to_client(int code, int type);
void refresh_fb();

// window list and related
struct window window_list[20];
int pointer_to = 0;
int active_window = -1;

// mouse
int *y = 0;
int *x = 0;

int main(void) {

    system("clear");
    system("tput civis");

    struct termios term;
    tcgetattr(fileno(stdin), &term);

    term.c_lflag &= ~ECHO;
    tcsetattr(fileno(stdin), 0, &term);

    enabled = 1;
    int fbfd = read_fb("");
    ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);
    ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo);
    size_t size = vinfo.yres * finfo.line_length;
    uint8_t *fbp_m = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd,0);


    fb_init();
    mkfifo("/tmp/fb_wm", 0777);
    mkfifo("/tmp/client_fb_wm", 0777);
    drawrect(x, y, finfo.line_length, vinfo.yres, "80A6FF");
    x = vinfo.xres / 2;
    y = vinfo.yres / 2;
    struct input_event ev;

    int event = 0;
    pthread_t thread, thread2;
    int err, err2;
    err = pthread_create(&thread, NULL, thread_f, NULL);
    err2 = pthread_create(&thread2, NULL, client_thread, NULL);
    if (err)
    {
        printf("An error occured: %d", err);
        return 1;
    }
    if (err2)
    {
        printf("An error occured: %d", err);
        return 1;
    }
    pthread_join(thread, NULL);
    pthread_join(thread2, NULL);

    enabled = 0;
    term.c_lflag |= ECHO;
    tcsetattr(fileno(stdin), 0, &term);
    system("cnorm");
}

struct client_event read_client() {
    int fd1 = open("/tmp/client_fb_wm", O_RDONLY);
    struct client_event data;
    read(fd1,&data, sizeof(struct client_event));
    close(fd1);
    return data;
}

void mouse_handler(struct input_event ev) {
    //printf("%d, %d\n", x, y);
    //printf("%d", ev.type);
    //printf("%d\n", ev.type);
    if (ev.type == EV_REL) {
        refresh_fb();
        FILE *fd1 = fopen("/home/proff/refresh","a+");
        if (ev.code == REL_X) {zzzzzzzzz
            if (x + ev.value < vinfo.xres) {
                fprintf(fd1, "x: %d\n", ev.value);
                x = x + ev.value;
            }
        } else if(ev.code == REL_Y){// y
            if (y + ev.value <= vinfo.yres) {
                fprintf(fd1, "y: %d\n", ev.value);
                y = y + ev.value;
            }
        }
        drawpixel(x, y, "E32636");
        fclose(fd1);
    }
    if(ev.type == EV_KEY && ev.code == BTN_MOUSE) {
        int fd1 = open("/tmp/fb_wm",O_WRONLY);
        struct server_event *serverEvent;
        serverEvent = malloc(sizeof(struct server_event));
        serverEvent->type = 1; // mouse event
        serverEvent->inputEventImproved.event = 2;
        serverEvent->inputEventImproved.x = *x;
        serverEvent->inputEventImproved.y = *y;
        if(ev.value == 1) {
            serverEvent->inputEventImproved.isPressed = 1;
        } else {
            serverEvent->inputEventImproved.isPressed = 0;
        }
        write(fd1, serverEvent, sizeof(struct server_event));
        close(fd1);
    } else if(ev.type == EV_KEY && ev.code == BTN_RIGHT) {
        int fd1 = open("/tmp/fb_wm",O_WRONLY);
        struct server_event *serverEvent;
        serverEvent->type = 1;
        serverEvent = malloc(sizeof(struct server_event));
        serverEvent->inputEventImproved.event = 1;
        serverEvent->inputEventImproved.x = *x;
        serverEvent->inputEventImproved.y = *y;
        if(ev.value == 1) {
            serverEvent->inputEventImproved.isPressed = 1;
        } else {
            serverEvent->inputEventImproved.isPressed = 0;
        }
        write(fd1, serverEvent, sizeof(struct server_event));
        close(fd1);
    }
}

void refresh_fb() {
    drawrect(0, 0, finfo.line_length, vinfo.yres, "87CEEB");
    if(pointer_to > 0) {
        for(int i = 0; i < pointer_to; i++) {
            if(i == active_window) {
                continue;
            }
            struct window win = window_list[i];
            drawrect(win.x, win.y, win.x1, win.y1, win.color);
            send_data_to_client(2, window_list[i].id);
        }
        struct window active_win = window_list[active_window];
        drawrect(active_win.x, active_win.y, active_win.x1, active_win.y1, active_win.color);
        send_data_to_client(2, window_list[active_window].id);

    }
}

void send_data_to_client(int code, int id) {
    int fd1 = open("/tmp/fb_wm",O_WRONLY);
    struct server_event *serverEvent;
    serverEvent = malloc(sizeof(struct server_event));
    serverEvent->type = code;
    serverEvent->id = id;
    write(fd1, serverEvent, sizeof(struct server_event));
    close(fd1);
}

void thread_f(void) {
    struct input_event ev;
    int fd;
    const char* pFile = "/dev/input/by-id/usb-Logitech_USB_Optical_Mouse-event-mouse";
    fd = open(pFile, O_RDWR);
    while (enabled) {
        read(fd,&ev,sizeof(struct input_event));
        mouse_handler(ev);
    }
    close(fd);
}

void client_thread(void) {
    while (enabled) {
        struct client_event client = read_client();
        if(client.type == 1) {

            //printf("Creating window[%d]", client.win.id);
            window_list[pointer_to] = client.win;
            active_window = pointer_to;
            pointer_to += 1;
            refresh_fb();
        }
        //printf("x: %d, y: %d\n", x, y);
    }
}