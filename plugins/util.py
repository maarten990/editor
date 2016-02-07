import re


class cursor_at:
    def __init__(self, buf, x, y):
        self.buf = buf
        self.x = x
        self.y = y

    def __enter__(self):
        self.old_x, self.old_y = self.buf.cursor
        self.buf.cursor = (self.x, self.y)

    def __exit__(self, type, value, traceback):
        self.buf.cursor = (self.old_x, self.old_y)


def kill_line():
    buf = current_buffer()

    _, y = buf.cursor
    buf.cursor = (999, y)

    x, _ = buf.cursor
    buf.delete_backwards(x)


def newline_and_indent():
    buf = current_buffer()
    x, y = buf.cursor

    match = re.search(r'\w', buf.get_line(y))
    n_spaces = match.start() if match else 0
    buf.insert('\n' + (' ' * n_spaces))
