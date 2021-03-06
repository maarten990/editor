from editor import current_buffer
from util import *
from multiple_cursors import *

TB_KEY_F1               = (0xFFFF-0)
TB_KEY_F2               = (0xFFFF-1)
TB_KEY_F3               = (0xFFFF-2)
TB_KEY_F4               = (0xFFFF-3)
TB_KEY_F5               = (0xFFFF-4)
TB_KEY_F6               = (0xFFFF-5)
TB_KEY_F7               = (0xFFFF-6)
TB_KEY_F8               = (0xFFFF-7)
TB_KEY_F9               = (0xFFFF-8)
TB_KEY_F10              = (0xFFFF-9)
TB_KEY_F11              = (0xFFFF-10)
TB_KEY_F12              = (0xFFFF-11)
TB_KEY_INSERT           = (0xFFFF-12)
TB_KEY_DELETE           = (0xFFFF-13)
TB_KEY_HOME             = (0xFFFF-14)
TB_KEY_END              = (0xFFFF-15)
TB_KEY_PGUP             = (0xFFFF-16)
TB_KEY_PGDN             = (0xFFFF-17)
TB_KEY_ARROW_UP         = (0xFFFF-18)
TB_KEY_ARROW_DOWN       = (0xFFFF-19)
TB_KEY_ARROW_LEFT       = (0xFFFF-20)
TB_KEY_ARROW_RIGHT      = (0xFFFF-21)
TB_KEY_MOUSE_LEFT       = (0xFFFF-22)
TB_KEY_MOUSE_RIGHT      = (0xFFFF-23)
TB_KEY_MOUSE_MIDDLE     = (0xFFFF-24)
TB_KEY_MOUSE_RELEASE    = (0xFFFF-25)
TB_KEY_MOUSE_WHEEL_UP   = (0xFFFF-26)
TB_KEY_MOUSE_WHEEL_DOWN = (0xFFFF-27)
TB_KEY_CTRL_TILDE       = 0x00
TB_KEY_CTRL_2           = 0x00 # clash with 'CTRL_TILDE'
TB_KEY_CTRL_A           = 0x01
TB_KEY_CTRL_B           = 0x02
TB_KEY_CTRL_C           = 0x03
TB_KEY_CTRL_D           = 0x04
TB_KEY_CTRL_E           = 0x05
TB_KEY_CTRL_F           = 0x06
TB_KEY_CTRL_G           = 0x07
TB_KEY_BACKSPACE        = 0x08
TB_KEY_CTRL_H           = 0x08 # clash with 'CTRL_BACKSPACE'
TB_KEY_TAB              = 0x09
TB_KEY_CTRL_I           = 0x09 # clash with 'TAB'
TB_KEY_CTRL_J           = 0x0A
TB_KEY_CTRL_K           = 0x0B
TB_KEY_CTRL_L           = 0x0C
TB_KEY_ENTER            = 0x0D
TB_KEY_CTRL_M           = 0x0D # clash with 'ENTER'
TB_KEY_CTRL_N           = 0x0E
TB_KEY_CTRL_O           = 0x0F
TB_KEY_CTRL_P           = 0x10
TB_KEY_CTRL_Q           = 0x11
TB_KEY_CTRL_R           = 0x12
TB_KEY_CTRL_S           = 0x13
TB_KEY_CTRL_T           = 0x14
TB_KEY_CTRL_U           = 0x15
TB_KEY_CTRL_V           = 0x16
TB_KEY_CTRL_W           = 0x17
TB_KEY_CTRL_X           = 0x18
TB_KEY_CTRL_Y           = 0x19
TB_KEY_CTRL_Z           = 0x1A
TB_KEY_ESC              = 0x1B
TB_KEY_CTRL_LSQ_BRACKET = 0x1B # clash with 'ESC'
TB_KEY_CTRL_3           = 0x1B # clash with 'ESC'
TB_KEY_CTRL_4           = 0x1C
TB_KEY_CTRL_BACKSLASH   = 0x1C # clash with 'CTRL_4'
TB_KEY_CTRL_5           = 0x1D
TB_KEY_CTRL_RSQ_BRACKET = 0x1D # clash with 'CTRL_5'
TB_KEY_CTRL_6           = 0x1E
TB_KEY_CTRL_7           = 0x1F
TB_KEY_CTRL_SLASH       = 0x1F # clash with 'CTRL_7'
TB_KEY_CTRL_UNDERSCORE  = 0x1F # clash with 'CTRL_7'
TB_KEY_SPACE            = 0x20
TB_KEY_BACKSPACE2       = 0x7F
TB_KEY_CTRL_8           = 0x7F # clash with 'BACKSPACE2'

def set_mode(mode):
    if mode == 'insert':
        current_buffer.keymap = insert_mode_map
    if mode == 'normal':
        current_buffer.keymap = normal_mode_map

def forward_word():
    x, y = current_buffer.cursor
    line = current_buffer.get_line(y)
    offset = line[x+1:].find(' ')
    move_cursor(x=offset + 2)

def backward_word():
    x, y = current_buffer.cursor
    line = current_buffer.get_line(y)
    offset = line[:x+1][::-1].find(' ')
    move_cursor(x=-offset - 1)

class InsertKeymap(dict):
    def __missing__(self, key):
        return lambda: multi_cursor_insert(chr(key))

normal_mode_map = {
    ord('w'): lambda: forward_word(),
    ord('b'): lambda: backward_word(),
    ord('i'): lambda: set_mode('insert'),
    ord('l'): lambda: move_cursor(x=1),
    ord('h'): lambda: move_cursor(x=-1),
    ord('k'): lambda: move_cursor(y=-1),
    ord('j'): lambda: move_cursor(y=1)
}

insert_mode_map = InsertKeymap({
    TB_KEY_SPACE       : lambda: multi_cursor_insert(' '),
    TB_KEY_BACKSPACE   : lambda: multi_cursor_backspace(),
    TB_KEY_BACKSPACE2  : lambda: multi_cursor_backspace(),
    TB_KEY_SPACE       : lambda: multi_cursor_insert(' '),
    TB_KEY_ENTER       : lambda: newline_and_indent(),
    TB_KEY_CTRL_R      : lambda: add_multi_cursor(),
    TB_KEY_CTRL_W      : lambda: reset_multi_cursor(),
    TB_KEY_CTRL_K      : lambda: kill_line(),
    TB_KEY_ARROW_RIGHT : lambda: move_cursor(x=1),
    TB_KEY_ARROW_LEFT  : lambda: move_cursor(x=-1),
    TB_KEY_ARROW_UP    : lambda: move_cursor(y=-1),
    TB_KEY_ARROW_DOWN  : lambda: move_cursor(y=1),
    TB_KEY_CTRL_C      : lambda: set_mode('normal')
})

current_buffer.keymap = normal_mode_map
