#ifndef LINE_H
#define LINE_H

#include "gapbuffer.h"
#include "list.h"

struct Line {
    struct Gapbuffer *gapbuf;
    int cursor;

    struct list_head list;
    int is_head;
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

// return the previous/next lines in the list
struct Line *line_previous(struct Line *line);
struct Line *line_next(struct Line *line);

void line_delete_list(struct Line *line);

#endif /* LINE_H */
