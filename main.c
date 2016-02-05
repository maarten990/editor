#include <stdio.h>
#include "line.h"
#include "tui.h"
#include "python_bindings.h"

int main(int argc, char *argv[])
{
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
        printf("Error: too many arguments given.\n");
        return -1;
    }

    if (ui_init() != 0) {
        printf("Error initializing termbox\n");
        return -1;
    }

    python_init();

    ui_loop(buffer);

    python_destroy();
    ui_destroy();
    return 0;
}
