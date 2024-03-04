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
    int fd1 = open("/tmp/client_fb_wm",O_WRONLY);
    struct client_event clientEvent;
    clientEvent.type = event.type;
    clientEvent.win = event.win;
    
    write(fd1, &clientEvent, sizeof(clientEvent));
    close(fd1);
}

void refresh() {
    return;
}