#include <stdlib.h>
#include "line.h"

struct Line *line_new(rune *contents)
{
    struct Line *line = malloc(sizeof(struct Line));

    line->gapbuf = gapbuffer_new(contents, 16);
    line->cursor = line->gapbuf->gapstart;

    return line;
}

void line_free(struct Line *line)
{
    gapbuffer_free(line->gapbuf);
    free(line);
}

int line_move_cursor(struct Line *line, int offset)
{
    int moved = gapbuf_move_cursor(line->gapbuf, offset);
    line->cursor += moved;

    return moved;
}

void line_insert_char(struct Line *line, rune ch)
{
    gapbuf_insert_char(line->gapbuf, ch);
    line->cursor += 1;
}

rune *line_display(struct Line *line)
{
    gapbuf_display(line->gapbuf);
    return line->gapbuf->disp_buffer;
}

int line_backspace(struct Line *line)
{
    int deleted = gapbuf_delete_backwards(line->gapbuf, 1);
    line->cursor -= deleted;

    return deleted;
}
