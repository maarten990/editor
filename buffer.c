#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "buffer.h"
#include "util.h"

struct Buffer *buffer_new()
{
    struct Buffer *buffer = malloc(sizeof(struct Buffer));
    buffer->lines = lines_new();
    buffer->current_line = buffer->lines->first;
    buffer->cursor_x = 0;
    buffer->cursor_y = 0;
    buffer->filename = NULL;

    struct View view = {
        .width = -1,
        .height = -1,
        .start = 0,
    };

    buffer->view = view;

    return buffer;
}

void buffer_insert(struct Buffer *buf, rune ch)
{
    line_insert_char(buf->current_line, ch);
    buf->cursor_x += 1;
}

void buffer_delete_backwards(struct Buffer *buf, int n)
{
    int deleted = line_delete_backwards(buf->current_line, n);
    buf->cursor_x -= deleted;

    int diff = n - deleted;

    // join with the previous line if we're deleting past the start of the line
    if (diff != 0 && buf->current_line->previous != NULL) {
        rune *text = line_display(buf->current_line);
        int size = strlen(text);
        buffer_move_cursor_y(buf, -1);
        buffer_move_cursor_x(buf, buf->current_line->gapbuf->bufsize);

        // insert a space if the text is being appended after existing text
        if (buf->cursor_x > 0 && size > 0)
            buffer_insert(buf, ' ');

        for (int i = 0; i < size; ++i)
            buffer_insert(buf, text[i]);

        // move the cursor back to the right place
        buffer_move_cursor_x(buf, -size);

        lines_remove(buf->lines, buf->current_line->next);
    }
}

void buffer_move_cursor_x(struct Buffer *buf, int offset)
{
    buf->cursor_x += line_move_cursor(buf->current_line, offset);
}

void buffer_move_cursor_y(struct Buffer *buf, int offset)
{
    // moving downwards
    if (offset > 0) {
        for (int i = 0; i < offset; ++i) {
            if (buf->current_line->next == NULL)
                break;

            buf->current_line = buf->current_line->next;
            buf->cursor_y += 1;
        }
    }
    
    // moving upwards
    else {
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

    // adjust the view if necessary
    if (offset > 0) {
        int too_far = (buf->cursor_y - buf->view.start) - buf->view.height;
        if (too_far >= 0)
            buf->view.start += too_far + 1;
    } else {
        int offset = (buf->cursor_y - buf->view.start);
        if (offset < 0)
            buf->view.start += offset;
    }
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

    // set the filename
    if (buf->filename)
        free(buf->filename);
    buf->filename = malloc(sizeof(char) * strlen(path) + 1);
    sprintf(buf->filename, "%s", path);

    return 0;
}

void buffer_clear(struct Buffer *buf)
{
    buf->current_line = NULL;
    lines_free(buf->lines);
    buf->lines = lines_new();
}

void buffer_break_at_cursor(struct Buffer *buf)
{
    rune *current_text = line_display(buf->current_line);
    rune newline_text[strlen(current_text)];

    strncpy(newline_text, current_text + buf->cursor_x, sizeof(newline_text));

    // ensure null termination
    newline_text[sizeof(newline_text) - 1] = '\0';

    // move cursor to the end of the buffer and backspace until the cutoff point
    line_move_cursor_abs(buf->current_line, buf->current_line->gapbuf->bufsize);
    buffer_delete_backwards(buf,
                            buf->current_line->cursor - buf->cursor_x);

    lines_add_after(buf->lines, buf->current_line, line_new(newline_text));

    buf->cursor_x = 0;
    buffer_move_cursor_y(buf, 1);
}

int buffer_write_to_file(struct Buffer *buf, const char *path)
{
    // write the buffer to a temporary file which replaces the original file if
    // there were no errors
    char temp_path[strlen(path) + 5]; // +4 for the prefix, +1 for \0
    sprintf(temp_path, ".%s.tmp", path);
    FILE *file = fopen(temp_path, "w");

    if (file == NULL)
        return -1;

    struct Line *line = buf->lines->first;
    char *disp;

    do {
        disp = line_display(line);
        fprintf(file, "%s\n", disp);
    } while ((line = line->next) != NULL);

    // backup the original file and move the tempfile in its place
    char backup[strlen(path) + 2]; // +1 for the suffix, +1 for \0
    sprintf(backup, "%s~", path);
    if (copy_file(path, backup) < 0)
        return -1;
    if (rename(temp_path, path) < 0)
        return errno;

    return 0;
}
