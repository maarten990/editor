#ifndef TUI_H
#define TUI_H

#include "buffer.h"

// initialize the UI
int ui_init();

// set the viewport of the buffer
void set_view(struct Buffer *buffer);

// enter the main loop of the UI
void ui_loop(struct Buffer *buffer);

// draw current state of the buffer
void ui_draw(struct Buffer *buffer);

// clean up the UI
void ui_destroy();

#endif /* TUI_H */
