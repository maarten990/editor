#ifndef LINE_H
#define LINE_H

#include "gapbuffer.h"

struct Line {
    struct Gapbuffer *gapbuf;
    int cursor;

    struct Line *next;
    struct Line *previous;
};

struct Lines {
    struct Line *first;
    struct Line *last;
};

struct Line *line_new(rune *contents);

void line_free(struct Line *line);

int line_move_cursor(struct Line *line, int offset);

void line_insert_char(struct Line *line, rune ch);

// delete 1 character backwards from the cursor
int line_backspace(struct Line *line);

// return a string container a representation of the line
rune *line_display(struct Line *line);

int line_move_cursor_abs(struct Line *line, int cursor);

// create a new list of lines
struct Lines *lines_new();

void lines_free(struct Lines *list);

void lines_add(struct Lines *list, struct Line *line);

void lines_add_after(struct Lines *list, struct Line *before,
                     struct Line *line);

void lines_remove(struct Lines *list, struct Line *line);

#endif /* LINE_H */
