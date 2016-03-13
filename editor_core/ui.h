#ifndef TUI_H
#define TUI_H

#include <Python.h>
#include "buffer.h"
#include "list.h"

extern struct list_head panes;
extern struct UI_Pane *active_pane;

struct UI_Pane {
    struct list_head list;

    struct Buffer *buf;

    // a dictionary mapping keycodes to callable PyObjects
    PyObject *keymap;

    int anchor_x;
    int anchor_y;
    int width_ratio;
    int height_ratio;
};

// initialize the UI
int ui_init();

// set the viewport of the buffer
void set_view(struct Buffer *buffer, int width, int height, int x, int y);

// enter the main loop of the UI
void ui_loop();

// add a buffer to the list of buffers
void ui_add_buffer(struct Buffer *buf, int make_active,
                   double width_ratio, double height_ratio,
                   int anchor_x, int anchor_y);

// draw current state of the buffer
void ui_draw();

// clean up the UI
void ui_destroy();

#endif /* TUI_H */
