#ifndef TUI_H
#define TUI_H

#include "buffer.h"

extern struct Buffer *active_buffer;

// initialize the UI
int ui_init(struct Buffer *buffer);

// set the viewport of the buffer
void set_view(struct Buffer *buffer);

// enter the main loop of the UI
void ui_loop();

// draw current state of the buffer
void ui_draw(struct Buffer *buffer);

// clean up the UI
void ui_destroy();

#endif /* TUI_H */
