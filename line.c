#include <stdlib.h>
#include <MacTypes.h>
#include "line.h"

struct Line *line_new(rune *contents)
{
    struct Line *line = malloc(sizeof(struct Line));

    line->gapbuf = gapbuffer_new(contents, 16);
    line->cursor = line->gapbuf->gapstart;

    line->next = NULL;
    line->previous = NULL;

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

int line_move_cursor_abs(struct Line *line, int cursor)
{
    int offset = cursor - line->cursor;
    return line_move_cursor(line, offset);
}

struct Lines *lines_new()
{
    struct Lines *lines = malloc(sizeof(struct Lines));
    lines->first = NULL;
    lines->last = NULL;

    return lines;
}

void lines_free(struct Lines *list)
{
    if (list->first != NULL) {
        struct Line *next;
        struct Line *line = list->first;

        do {
            next = line->next;
            line_free(line);
        } while (next != NULL);
    }

    free(list);
}

void lines_add(struct Lines *list, struct Line *line)
{
    // uninitialized list
    if (list->last == NULL) {
        list->first = line;
        list->last = line;
        return;
    }

    struct Line *previous_last = list->last;
    previous_last->next = line;
    line->previous = previous_last;
    list->last = line;
}

void lines_add_after(struct Lines *list, struct Line *before,
                     struct Line *line)
{
    struct Line *after = before->next;
    if (after != NULL)
        after->previous = line;

    before->next = line;

    line->previous = before;
    line->next = after;
}

void lines_remove(struct Lines *list, struct Line *line)
{
    line->previous = line->next;
    line->next = line->previous;

    line_free(line);
}

struct Line *lines_nth(struct Lines *list, int n)
{
    struct Line *elem = list->first;

    while (elem != NULL) {
        if (n == 0)
            return elem;

        n -= 1;
        elem = elem->next;
    }

    return NULL;
}
