#ifndef GAPBUFFER_H
#define GAPBUFFER_H

#include <wchar.h>

typedef wchar_t rune;

struct Gapbuffer {
    rune *buffer;
    rune *display;
    int gapsize;
    int gapstart;
    int gapend;
    int bufsize;
};

/**
 * Construct a new gapbuffer.
 */
struct Gapbuffer new_Gapbuffer(const char *contents, int gapsize);

/**
 * Free a gapbuffer's allocated memory.
 */
void free_Gapbuffer(struct Gapbuffer *buf);

/**
 * If the gap is closed, recreate it at the full gapsize.
 */
void ensure_gapsize(struct Gapbuffer *buf);

/**
 * Insert a single character at the first point in the gap.
 * @param ch a character to be inserted
 */
void insert_char(struct Gapbuffer *buf, rune ch);

/**
 * Delete characters backwards starting from the gap's opening.
 * @param n_chars the number of characters to delete
 */
void delete_backwards(struct Gapbuffer *buf, int n_chars);

/**
 * Move the cursor by a given offset.
 * @param offset The offset to move. Positive moves to the right, negative
 * to the left.
 *
 * @return The number of positions the cursor succesfully moved.
 */
int move_cursor(struct Gapbuffer *buf, int offset);

/**
 * Return a string representation of the entire buffer.
 */
rune *display(struct Gapbuffer *buf);

#endif
