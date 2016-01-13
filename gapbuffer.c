#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gapbuffer.h"

void memcpy_rune_to_char(char *dest, const rune *src, int runes)
{
    for (int i = 0; i < runes; ++i) {
        dest[i] = src[i];
    }
}

void memcpy_char_to_rune(rune *dest, const char *src, int chars)
{
    for (int i = 0; i < chars; ++i) {
        dest[i] = src[i];
    }
}

struct Gapbuffer new_Gapbuffer(const char *contents, int gapsize)
{
    int content_len = strlen(contents);

    struct Gapbuffer buf = {
        .buffer = calloc(content_len + gapsize, sizeof(rune)),
        .display = NULL,
        .gapsize = gapsize,
        .gapstart = 0,
        .gapend = gapsize,
        .bufsize = content_len + gapsize,
    };

    // fill the buffer starting at the end of the gap
    memcpy_char_to_rune(buf.buffer + buf.gapend, contents, content_len);

    return buf;
}

void free_Gapbuffer(struct Gapbuffer *buf)
{
    free(buf->buffer);
}

void ensure_gapsize(struct Gapbuffer *buf)
{
    if (buf->gapend == buf->gapstart) {
        buf->buffer = realloc(buf->buffer,
                              sizeof(rune) * (buf->bufsize + buf->gapsize));
        buf->gapend += buf->gapsize;
        buf->bufsize += buf->gapsize;

        // iterate backwards through the buffer and shift each element right by
        // the gapsize
        for (int i = buf->bufsize - buf->gapsize; i >= buf->gapstart; --i) {
            buf->buffer[i + buf->gapsize] = buf->buffer[i];
        }
        /*
        for (auto it = buffer.end() - gapsize; it != gapstart_it() - 1; --it) {
            *(it + gapsize) = *it;
        }
        */
    }
}

void insert_char(struct Gapbuffer *buf, rune ch)
{
    ensure_gapsize(buf);

    buf->buffer[buf->gapstart] = ch;
    buf->gapstart += 1;
}

void delete_backwards(struct Gapbuffer *buf, int n_chars)
{
    for (int i = 0; i < n_chars; ++i) {
        if (buf->gapstart > 0) {
            buf->gapstart -= 1;
        }
    }
}

int move_cursor(struct Gapbuffer *buf, int offset)
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

rune *display(struct Gapbuffer *buf)
{
    buf->display = realloc(buf->display,
                           sizeof(rune) * (buf->bufsize - buf->gapsize) + 1);

    int out_idx = 0;
    for (int i = 0; i < buf->bufsize; i++) {
        if (i >= buf->gapstart && i < buf->gapend)
            continue;
        else {
            buf->display[out_idx] = buf->buffer[i];
            out_idx += 1;
        }
    }

    buf->display[out_idx] = '\0';

    return buf->display;
}
