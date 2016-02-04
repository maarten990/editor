#ifndef EDITOR_BUFFER_H
#define EDITOR_BUFFER_H

#include "line.h"

struct Buffer {
    struct Lines *lines;
    struct Line *current_line;
    int cursor_x;
    int cursor_y;

    struct View {
        int width;
        int height;
        int start;
    } view;
};

// allocate a new buffer
struct Buffer *buffer_new();

// free a previously allocated buffer
void buffer_free(struct Buffer *buf);

// insert ch at the current cursor position
void buffer_insert(struct Buffer *buf, rune ch);

// remove n characters backwards from the cursor position
void buffer_delete_backwards(struct Buffer *buf, int n);

// move the cursor right by the given offset (negative values move left)
void buffer_move_cursor_x(struct Buffer *buf, int offset);

// move the cursor down by the given offset (negative values move up)
void buffer_move_cursor_y(struct Buffer *buf, int offset);

// add a new line after the current line
void buffer_add_line(struct Buffer *buf, struct Line *line);

// read a file's contents into the buffer
int buffer_read_file(struct Buffer *buf, const char *path);

// clear the contents of the buffer
void buffer_clear(struct Buffer *buf);

// end the current line at the cursor and move the remainder to a new line
void buffer_break_at_cursor(struct Buffer *buf);

// write the buffer's contents to a file
void buffer_write_to_file(struct Buffer *buf, const char *path);

#endif //EDITOR_BUFFER_H
