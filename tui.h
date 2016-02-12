#ifndef TUI_H
#define TUI_H

#include "buffer.h"
#include "list.h"

extern struct list_head buffers;
extern struct Buffer *active_buffer;

// initialize the UI
int ui_init();

// set the viewport of the buffer
void set_view(struct Buffer *buffer, int offset_x, int offset_y, int width,
              int height, int x, int y);

// enter the main loop of the UI
void ui_loop();

// add a buffer to the list of buffers
void ui_add_buffer(struct Buffer *buf);

// draw current state of the buffer
void ui_draw();

// clean up the UI
void ui_destroy();

#endif /* TUI_H */
