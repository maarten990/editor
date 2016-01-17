#ifndef LINE_H
#define LINE_H

#include "gapbuffer.h"

struct Line {
    struct Gapbuffer *gapbuf;
    int cursor;
};

struct Line *line_new(rune *contents);

void line_free(struct Line *line);

int line_move_cursor(struct Line *line, int offset);

void line_insert_char(struct Line *line, rune ch);

// delete 1 character backwards from the cursor
int line_backspace(struct Line *line);

// return a string container a representation of the line
rune *line_display(struct Line *line);

#endif /* LINE_H */
