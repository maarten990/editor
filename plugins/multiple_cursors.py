from editor import current_buffer

current_buffer.cursors = []

def add_multi_cursor():
    current_buffer.cursors.append(current_buffer.cursor)


def reset_multi_cursor():
    current_buffer.cursors = []


def multi_cursor_insert(s):
    current_buffer.insert(s)

    for i, cursor in enumerate(current_buffer.cursors):
        x, y = cursor

        with cursor_at(current_buffer, x, y):
            current_buffer.insert(s)
            current_buffer.cursors[i] = (x + 1, y)


def multi_cursor_backspace():
    current_buffer.delete_backwards(1)

    buf = current_buffer

    for i, cursor in enumerate(current_buffer.cursors):
        x, y = cursor

        with cursor_at(current_buffer, x, y):
            current_buffer.delete_backwards(1)
            current_buffer.cursors[i] = (x - 1 if x > 0 else x, y)
