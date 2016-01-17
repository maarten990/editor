#include <stdio.h>
#include "line.h"
#include "tui.h"

int main(int argc, char *argv[])
{
    ui_init();

    // TODO: replace the single line with a linked list of lines or something
    struct Line *line = line_new("Hello World!");
    ui_loop(line);

    ui_destroy();
    return 0;
}
