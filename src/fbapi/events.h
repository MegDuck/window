//
// Created by proff on 17.04.2022.
//

#pragma once

struct cinput_event {
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
    struct cinput_event inputEvent;
    struct window win;
};