from editor import current_buffer
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
    _, y = current_buffer.cursor
    current_buffer.cursor = (999, y)

    x, _ = current_buffer.cursor
    current_buffer.delete_backwards(x)


def newline_and_indent():
    x, y = current_buffer.cursor

    match = re.search(r'\w', current_buffer.get_line(y))
    n_spaces = match.start() if match else 0
    current_buffer.insert('\n' + (' ' * n_spaces))
