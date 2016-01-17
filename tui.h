#include "buffer.h"

// initialize the UI
int ui_init();

// enter the main loop of the UI
void ui_loop(struct Buffer *buffer);

// draw current state of the buffer
void ui_draw(struct Buffer *buffer);

// clean up the UI
void ui_destroy();
