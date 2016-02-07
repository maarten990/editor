import editor

def toggle_multi_cursor_cursor():
    buf = editor.current_buffer

    if not hasattr(buf, 'second_cursor'):
        x, y = buf.cursor
        buf.second_cursor = (x, y)
    else:
        delattr(f, 'second_cursor')


def multi_cursor_insert(s):
    buf = editor.current_buffer

    if hasattr(buf, 'second_cursor'):
        x, y = buf.second_cursor
        with cursor_at(buf, x, y):
            buf.insert(s)
            buf.second_cursor = (x + 1, y)

    buf.insert(s)


def multi_cursor_backspace():
    buf = editor.current_buffer

    if hasattr(buf, 'second_cursor'):
        x, y = buf.second_cursor
        with cursor_at(buf, x, y):
            buf.delete_backwards(1)
            buf.second_cursor = (x - 1 if x > 0 else x, y)

    buf.delete_backwards(1)
