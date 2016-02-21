#include <stdio.h>
#include <termbox.h>
#include "line.h"
#include "tui.h"
#include "python_bindings.h"

int main(int argc, char *argv[])
{
    if (ui_init() != 0) {
        printf("Error initializing termbox\n");
        return -1;
    }

    // no arguments, use some dummy data
    if (argc == 1) {
        struct Buffer *buf = buffer_new();
        buffer_add_line(buf, line_new("Hello World!"));
        buffer_add_line(buf, line_new("This is line 2."));
        buffer_add_line(buf, line_new("I am a duck."));
        ui_add_buffer(buf, 1, 1, 1, 0, 0);
    } else {
        int width = tb_width() / (argc - 1);
        for (int i = 1; i < argc; ++i) {
            struct Buffer *buf = buffer_new();
            int err = buffer_read_file(buf, argv[i]);

            if (err) {
                printf("Error opening file: %s\n", argv[1]);
                continue;
            } else {
                ui_add_buffer(buf, 1,
                              1.0 / (float)(argc - 1), 1,
                              (i-1) * width, 0);
            }
        }
    }

    python_init();
    ui_loop();

    python_destroy();
    ui_destroy();
    return 0;
}
