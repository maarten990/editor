#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "buffer.h"
#include "util.h"

struct Buffer *buffer_new(const char *contents)
{
    struct Buffer *buffer = malloc(sizeof(struct Buffer));

    buffer->gapbuf = gapbuffer_new(contents, 16);
    buffer->cursor_x = buffer->gapbuf->gapstart;
    buffer->cursor_y = 0;

    return buffer;
}

void buffer_free(struct Buffer *buffer)
{
    gapbuffer_free(buffer->gapbuf);
    free(buffer);
}

void buffer_insert(struct Buffer *buf, rune ch)
{
    gapbuf_insert_char(buf->gapbuf, ch);
    buf->cursor_x += 1;
}

void buffer_delete_backwards(struct Buffer *buf, int n)
{
    int deleted = gapbuf_delete_backwards(buf->gapbuf, n);
    buf->cursor_x -= deleted;

    return deleted;
}

void buffer_move_cursor_x(struct Buffer *buf, int offset)
{
    int moved = gapbuf_move_cursor(buf->gapbuf, offset);
    buf->cursor_x += moved;
}

void buffer_move_cursor_y(struct Buffer *buf, int offset)
{
    // TODO
}

void buffer_add_line(struct Buffer *buf, struct Line *line)
{
    // TODO
}

// update the view to keep the cursor within bounds
static void buffer_update_view(struct Buffer *buf)
{
    // check horizontally
    int too_far = (buf->cursor_x - buf->view.start_x) - buf->view.width;
    if (too_far >= 0)
        buf->view.start_x += too_far + 1;
    else {
        int offset = (buf->cursor_x - buf->view.start_x);
        if (offset < 0)
            buf->view.start_x += offset;
    }

    // check vertically
    too_far = (buf->cursor_y - buf->view.start_y) - buf->view.height;
    if (too_far >= 0) {
        buf->view.start_y += too_far + 1;
    } else {
        int offset = (buf->cursor_y - buf->view.start_y);
        if (offset < 0)
            buf->view.start_y += offset;
    }
}


struct Line *buffer_nth_line(struct Buffer *buf, int n)
{
    // TODO

    return NULL;
}

int buffer_read_file(struct Buffer *buf, const char *path)
{
    // TODO
    return 0;

    /*
    FILE *file = fopen(path, "r");

    if (file == NULL)
        return -1;

    if (buf->current_line != NULL)
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
    */
}

void buffer_clear(struct Buffer *buf)
{
    // TODO
}

void buffer_break_at_cursor(struct Buffer *buf)
{
    // TODO
    return;

    /*
    rune *current_text = line_display(buf->current_line);
    rune newline_text[strlen(current_text)];

    strncpy(newline_text, current_text + buf->cursor_x, sizeof(newline_text));

    // ensure null termination
    newline_text[sizeof(newline_text)] = '\0';

    // move cursor to the end of the buffer and backspace until the cutoff point
    line_move_cursor_abs(buf->current_line, buf->current_line->gapbuf->bufsize);
    buffer_delete_backwards(buf,
                            buf->current_line->cursor - buf->cursor_x);

    struct Line *new = line_new(newline_text);
    list_add(&new->list, &buf->current_line->list);

    buffer_move_cursor_x(buf, -buf->cursor_x);
    buffer_move_cursor_y(buf, 1);
    buf->view.start_x = 0; // TODO: figure out why this is necessary and fix it
    */
}

int buffer_write_to_file(struct Buffer *buf, const char *path)
{
    // TODO

    /*
    buf->view.status_message = "Saving...";
    // write the buffer to a temporary file which replaces the original file if
    // there were no errors
    char temp_path[strlen(path) + 5]; // +4 for the prefix, +1 for \0
    sprintf(temp_path, ".%s.tmp", path);
    FILE *file = fopen(temp_path, "w");

    if (file == NULL) {
        fclose(file);
        return -1;
    }

	struct Line *pos;
    char *disp;
	
	list_for_each_entry(pos, &buf->head.list, list) {
		disp = line_display(pos);
		fprintf(file, "%s\n", disp);
	}

    fclose(file);

    // backup the original file and move the tempfile in its place
    char backup[strlen(path) + 2]; // +1 for the suffix, +1 for \0
    sprintf(backup, "%s~", path);
    if (copy_file(path, backup) < 0)
        return -1;
    if (rename(temp_path, path) < 0)
        return errno;

    buf->view.status_message = "File saved";
    return 0;
    */
}
