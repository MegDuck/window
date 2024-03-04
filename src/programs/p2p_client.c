#include <stdio.h>
#include <fcntl.h>
#include "../fbapi/events.h"
#include <stdlib.h>
#include "../window/window-codes.h"
// p2p
struct client_event data;
void refresh();

struct server_event get_info_from_server() {
    int fd1 = open("/tmp/fb_wm", O_RDONLY);
    struct server_event data;
    read(fd1,&data, sizeof(struct server_event));
    close(fd1);
    return data;
}

void send_info_to_server(struct client_event event) {
    int fd1 = open("/tmp/client_fb_wm", O_WRONLY);

    struct client_event clientEvent;
    clientEvent.type = event.type;
    clientEvent.win = event.win;
    write(fd1, clientEvent, sizeof(clientEvent));
    close(fd1);
}

int main(int argc, char *argv[]) {
    data.type = 1;
    data.win.x = 1;
    data.win.x1 = 100;
    data.win.y = 1;
    data.win.y1 = 100;

    data.win.id = 1;
    data.win.name = "first program";
    data.need_redraw = 1;
    data.win.color = "FFFF00";
    // creating window
    send_info_to_server(data);
    struct server_event info;
    while (1) {
        info = get_info_from_server();
        if(info.type == 1) {
            if(info.inputEvent.event == 1) {
                //printf("user clicked the right mouse button!\n");
            } else if(info.type == 2 && info.id == data.win.id) {
                //refresh
                refresh();
            }
        }

    }

}

void refresh() {
    return;
}

void interrupt() {
    return;
}