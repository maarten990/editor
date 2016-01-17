#include "line.h"

// initialize the UI
int ui_init();

// enter the main loop of the UI
void ui_loop(struct Lines *buffer);

// draw current state of the buffer
void ui_draw(struct Lines *buffer);

// clean up the UI
void ui_destroy();
