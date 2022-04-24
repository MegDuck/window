//
// Created by proff on 17.04.2022.
//

#ifndef WINDOW_INPUT_EVENT_H
#define WINDOW_INPUT_EVENT_H

struct input_event_improved {
    int x;
    int y;
    int event;
    int isPressed;
};


struct window {
    int x;
    int y;
    int x1;
    int y1;
    int id;
    char *color;
    char *name;
};

struct client_event {
    int type;
    int need_redraw;
    struct window win;
};

struct server_event {
    int id;
    int type;
    struct input_event_improved inputEventImproved;
    struct window win;
};

#endif //WINDOW_INPUT_EVENT_H
