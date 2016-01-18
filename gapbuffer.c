#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gapbuffer.h"

struct Gapbuffer *gapbuffer_new(rune *contents, int gapsize)
{
    int content_len = strlen(contents);
    struct Gapbuffer *buf = malloc(sizeof(struct Gapbuffer));

    buf->buffer = calloc(content_len + gapsize, sizeof(rune));
    buf->disp_buffer = NULL;
    buf->gapsize = gapsize;
    buf->gapstart = 0;
    buf->gapend = gapsize;
    buf->bufsize = content_len + gapsize;

    // fill the buffer starting at the end of the gap
    for (int i = 0; i < content_len; ++i) {
        (buf->buffer + buf->gapend)[i] = contents[i];
    }

    return buf;
}

void gapbuffer_free(struct Gapbuffer *buf)
{
    free(buf->buffer);
    free(buf);
}

void gapbuf_ensure_gapsize(struct Gapbuffer *buf)
{
    if (buf->gapend == buf->gapstart) {
        char *newbuf = realloc(buf->buffer,
                               sizeof(rune) * (buf->bufsize + buf->gapsize));

        // copy the data if we received a new pointer
        if (buf->buffer != newbuf)
            memcpy(newbuf, buf->buffer, buf->bufsize);

        buf->buffer = newbuf;
        buf->gapend += buf->gapsize;
        buf->bufsize += buf->gapsize;

        // iterate backwards through the buffer and shift each element right by
        // the gapsize
        for (int i = buf->bufsize - buf->gapsize; i >= buf->gapstart; --i) {
            buf->buffer[i + buf->gapsize] = buf->buffer[i];
        }
    }
}

void gapbuf_insert_char(struct Gapbuffer *buf, rune ch)
{
    gapbuf_ensure_gapsize(buf);

    buf->buffer[buf->gapstart] = ch;
    buf->gapstart += 1;
}

int gapbuf_delete_backwards(struct Gapbuffer *buf, int n_chars)
{
    int n_deleted = 0;

    for (int i = 0; i < n_chars; ++i) {
        if (buf->gapstart > 0) {
            buf->gapstart -= 1;
            n_deleted += 1;
        }
    }

    return n_deleted;
}

int gapbuf_move_cursor(struct Gapbuffer *buf, int offset)
{
    int n_moved = 0;

    if (offset > 0) {
        for (int i = offset; i > 0; --i) {
            if (buf->gapend == buf->bufsize) {
                break;
            }

            buf->buffer[buf->gapstart] = buf->buffer[buf->gapend];
            buf->gapstart += 1;
            buf->gapend += 1;

            n_moved += 1;
        }
    } else {
        for (int i = offset; i < 0; ++i ) {
            if (buf->gapstart == 0) {
                break;
            }

            buf->gapstart -= 1;
            buf->gapend -= 1;
            buf->buffer[buf->gapend] = buf->buffer[buf->gapstart];

            n_moved -= 1;
        }
    }

    return n_moved;
}

rune *gapbuf_display(struct Gapbuffer *buf)
{
    rune *new_disp = realloc(buf->disp_buffer,
                             sizeof(rune) * (buf->bufsize - buf->gapsize + 1));

    if (new_disp == NULL)
        printf("Error allocating display buffer");
    else
        buf->disp_buffer = new_disp;

    int out_idx = 0;
    for (int i = 0; i < buf->bufsize; i++) {
        if (i >= buf->gapstart && i < buf->gapend)
            continue;
        else {
            buf->disp_buffer[out_idx] = buf->buffer[i];
            out_idx += 1;
        }
    }

    buf->disp_buffer[out_idx] = '\0';

    return buf->disp_buffer;
}
