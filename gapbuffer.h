#ifndef GAPBUFFER_H
#define GAPBUFFER_H

#include <wchar.h>

typedef char rune;

struct Gapbuffer {
    rune *data;
    rune *disp_buffer;
    int gapsize;
    int gapstart;
    int gapend;
    int bufsize;
};

/**
 * Construct a new gapbuffer.
 */
struct Gapbuffer *gapbuffer_new(rune *contents, int gapsize);

/**
 * Free a gapbuffer's allocated memory.
 */
void gapbuffer_free(struct Gapbuffer *buf);

/**
 * If the gap is closed, recreate it at the full gapsize.
 */
void gapbuf_ensure_gapsize(struct Gapbuffer *buf);

/**
 * Insert a single character at the first point in the gap.
 * @param ch a character to be inserted
 */
void gapbuf_insert_char(struct Gapbuffer *buf, rune ch);

/**
 * Delete characters backwards starting from the gap's opening.
 * @param n_chars the number of characters to delete
 *
 * @return The number of positions the cursor succesfully moved backwards.
 */
int gapbuf_delete_backwards(struct Gapbuffer *buf, int n_chars);

/**
 * Move the cursor by a given offset.
 * @param offset The offset to move. Positive moves to the right, negative
 * to the left.
 *
 * @return The number of positions the cursor succesfully moved.
 */
int gapbuf_move_cursor(struct Gapbuffer *buf, int offset);

/**
 * Return a string representation of the entire buffer.
 */
rune *gapbuf_display(struct Gapbuffer *buf);

#endif
