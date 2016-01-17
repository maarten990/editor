#include <stdio.h>
#include "line.h"
#include "tui.h"

int main(int argc, char *argv[])
{
    if (ui_init() != 0) {
        printf("Error initializing termbox\n");
        return -1;
    }

    struct Buffer *buffer = buffer_new();
    buffer_add_line(buffer, line_new("Hello World!"));
    buffer_add_line(buffer, line_new("This is line 2."));
    buffer_add_line(buffer, line_new("I am a duck."));
    ui_loop(buffer);

    ui_destroy();
    return 0;
}
