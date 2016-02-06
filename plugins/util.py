import re

def newline_and_indent():
    buffer = current_buffer()
    x, y = buffer.cursor()

    match = re.search(r'\w', buffer.get_line(y))
    n_spaces = match.start() if match else 0
    buffer.insert('\n' + (' ' * n_spaces))

