#include "line.h"

// initialize the UI
void ui_init();

// enter the main loop of the UI
void ui_loop(struct Line *line);

// draw current state of the buffer
void ui_draw(struct Line *line);

// clean up the UI
void ui_destroy();
