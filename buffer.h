#ifndef EDITOR_BUFFER_H
#define EDITOR_BUFFER_H

#include "line.h"

struct Buffer {
    struct Lines *lines;
    struct Line *current_line;
    int cursor_x;
    int cursor_y;
};

struct Buffer *buffer_new();
void buffer_free(struct Buffer *buf);
void buffer_insert(struct Buffer *buf, rune ch);
void buffer_delete_backwards(struct Buffer *buf, int n);
void buffer_move_cursor_x(struct Buffer *buf, int offset);
void buffer_move_cursor_y(struct Buffer *buf, int offset);
void buffer_add_line(struct Buffer *buf, struct Line *line);
int buffer_read_file(struct Buffer *buf, const char *path);
void buffer_clear(struct Buffer *buf);
void buffer_break_at_cursor(struct Buffer *buf);

#endif //EDITOR_BUFFER_H
