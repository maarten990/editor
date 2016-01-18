#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>

struct Buffer *buffer_new()
{
    struct Buffer *buffer = malloc(sizeof(struct Buffer));
    buffer->lines = lines_new();
    buffer->current_line = buffer->lines->first;
    buffer->cursor_x = 0;
    buffer->cursor_y = 0;

    return buffer;
}

void buffer_insert(struct Buffer *buf, rune ch)
{
    line_insert_char(buf->current_line, ch);
    buf->cursor_x += 1;
}

void buffer_backspace(struct Buffer *buf)
{
    line_backspace(buf->current_line);
    buf->cursor_x -= 1;
}

void buffer_move_cursor_x(struct Buffer *buf, int offset)
{
    buf->cursor_x += line_move_cursor(buf->current_line, offset);
}

void buffer_move_cursor_y(struct Buffer *buf, int offset)
{
    if (offset > 0) {
        for (int i = 0; i < offset; ++i) {
            if (buf->current_line->next == NULL)
                break;

            buf->current_line = buf->current_line->next;
            buf->cursor_y += 1;
        }
    } else {
        for (int i = 0; i > offset; --i) {
            if (buf->current_line->previous == NULL)
                break;

            buf->current_line = buf->current_line->previous;
            buf->cursor_y -= 1;
        }
    }

    // move the cursor of the line to the global cursor location, and make sure
    // it's in range of the line
    line_move_cursor_abs(buf->current_line, buf->cursor_x);
    buf->cursor_x = buf->current_line->cursor;
}

void buffer_free(struct Buffer *buf)
{
    lines_free(buf->lines);
    free(buf);
}

void buffer_add_line(struct Buffer *buf, struct Line *line)
{
    lines_add(buf->lines, line);

    if (buf->current_line == NULL) {
        buf->current_line = line;
        buf->cursor_y = 0;
    }
}

int buffer_read_file(struct Buffer *buf, const char *path)
{
    FILE *file = fopen(path, "r");

    if (file == NULL)
        return -1;

    if (buf->lines->first != NULL)
        buffer_clear(buf);

    ssize_t bytes_read;
    size_t len;
    char *line = NULL;

    while ((bytes_read = getline(&line, &len, file)) != -1) {
        if (line[bytes_read - 1] == '\n')
            line[bytes_read - 1] = '\0';
        
        buffer_add_line(buf, line_new(line));
    }

    free(line);
    return 0;
}

void buffer_clear(struct Buffer *buf)
{
    buf->current_line = NULL;
    lines_free(buf->lines);
    buf->lines = lines_new();
}
