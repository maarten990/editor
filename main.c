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

    // no arguments, use some dummy data
    if (argc == 1) {
        buffer_add_line(buffer, line_new("Hello World!"));
        buffer_add_line(buffer, line_new("This is line 2."));
        buffer_add_line(buffer, line_new("I am a duck."));
    } else if (argc == 2) {
        int err = buffer_read_file(buffer, argv[1]);
        if (err) {
            printf("Error opening file: %s\n", argv[1]);
            return -1;
        }
    } else {
        printf("Error: too many arguments given.");
    }

    ui_loop(buffer);

    ui_destroy();
    return 0;
}
