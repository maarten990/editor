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

// move the cursor by the given offset, relative to its current position
int line_move_cursor(struct Line *line, int offset);

// insert a character at the current cursor position
void line_insert_char(struct Line *line, rune ch);

// delete 1 character backwards from the cursor
int line_delete_backwards(struct Line *line, int n);

// return a string container a representation of the line
rune *line_display(struct Line *line);

// move the cursor to the absolute position given by cursor
int line_move_cursor_abs(struct Line *line, int cursor);

struct Lines *lines_new();

void lines_free(struct Lines *list);

// append a line to the end of the list
void lines_add(struct Lines *list, struct Line *line);

// insert a line into the list after the given other line
void lines_add_after(struct Lines *list, struct Line *before,
                     struct Line *line);

// remove a given line from the list
void lines_remove(struct Lines *list, struct Line *line);

// return the n'th element of the list
struct Line *lines_nth(struct Lines *list, int n);

#endif /* LINE_H */
