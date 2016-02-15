#include <stdlib.h>
#include "line.h"
#include "list.h"

struct Line *line_new(rune *contents)
{
    struct Line *line = malloc(sizeof(struct Line));

    line->gapbuf = gapbuffer_new(contents, 16);
    line->cursor = line->gapbuf->gapstart;

    INIT_LIST_HEAD(&line->list);
    line->is_head = 0;
	
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

int line_delete_backwards(struct Line *line, int n)
{
    int deleted = gapbuf_delete_backwards(line->gapbuf, n);
    line->cursor -= deleted;

    return deleted;
}

int line_move_cursor_abs(struct Line *line, int cursor)
{
    int offset = cursor - line->cursor;
    return line_move_cursor(line, offset);
}

struct Line *line_previous(struct Line *line)
{
    struct Line *prev = list_entry(line->list.prev, struct Line, list);
    return prev->is_head ? NULL : prev;
}

struct Line *line_next(struct Line *line)
{
    struct Line *next = list_entry(line->list.next, struct Line, list);
    return next->is_head ? NULL : next;
}

void line_delete_list(struct Line *line)
{
    struct list_head *pos, *q;
    struct Line *tmp;

    list_for_each_safe(pos, q, &(line->list)) {
        tmp = list_entry(pos, struct Line, list);
        line_free(tmp);
    }
}
