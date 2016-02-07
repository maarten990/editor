buf = None


def toggle_multi_cursor_cursor():
    global buf

    if not buf:
        buf = current_buffer()

    if not hasattr(buf, 'second_cursor'):
        x, y = buf.cursor
        buf.second_cursor = (x, y)
    else:
        delattr(f, 'second_cursor')

    with open('python_log.txt', 'a') as f:
        f.write(str(buf.__dict__))
        f.write('\n')


def multi_cursor_insert(s):
    global buf

    if not buf:
        buf = current_buffer()

    with open('python_log.txt', 'a') as f:
        f.write(str(buf.__dict__))
        f.write('\n')

    if hasattr(buf, 'second_cursor'):
        x, y = buf.second_cursor
        with cursor_at(buf, x, y):
            buf.insert(s)
            buf.second_cursor = (x + 1, y)

    buf.insert(s)


def multi_cursor_backspace():
    global buf

    if not buf:
        buf = current_buffer()

    if hasattr(buf, 'second_cursor'):
        x, y = buf.second_cursor
        with cursor_at(buf, x, y):
            buf.delete_backwards(1)
            buf.second_cursor = (x - 1 if x > 0 else x, y)

    buf.delete_backwards(1)
