#include <locale.h>
#include <termbox.h>
#include <string.h>
#include "tui.h"

int ui_init()
{
    setlocale(LC_ALL, "");
    return tb_init();
}

void ui_loop(struct Buffer *buffer)
{
    ui_draw(buffer);

    while (1) {
        ui_draw(buffer);

        struct tb_event e;
        tb_poll_event(&e);

        char ch;
        switch (e.type) {
            case TB_EVENT_KEY:
                switch(e.key) {
                    case TB_KEY_ESC:
                        return;
                    case TB_KEY_ARROW_LEFT:
                        buffer_move_cursor_x(buffer, -1);
                        break;
                    case TB_KEY_ARROW_RIGHT:
                        buffer_move_cursor_x(buffer, 1);
                        break;
                    case TB_KEY_ARROW_UP:
                        buffer_move_cursor_y(buffer, -1);
                        break;
                    case TB_KEY_ARROW_DOWN:
                        buffer_move_cursor_y(buffer, 1);
                        break;
                    case TB_KEY_BACKSPACE:
                    case TB_KEY_BACKSPACE2:
                        buffer_delete_backwards(buffer, 1);
                        break;
                    case TB_KEY_SPACE:
                        buffer_insert(buffer, ' ');
                        break;
                    case TB_KEY_ENTER:
                        buffer_break_at_cursor(buffer);
                        break;
                    default:
                        if (e.ch > 0 && e.ch <= 128) {
                            tb_utf8_unicode_to_char(&ch, e.ch);
                            buffer_insert(buffer, ch);
                        }
                        break;
                }
                break;
            case TB_EVENT_RESIZE:
                break;
            default:
                break;
        }
    }
}

void ui_draw(struct Buffer *buffer)
{
    tb_clear();
    tb_select_output_mode(TB_OUTPUT_256);

    int row = 0;
    struct Line *line = buffer->lines->first;

    do {
        rune *disp = line_display(line);

        for (int col = 0; col < strlen(disp); ++col) {
            tb_change_cell(col, row, disp[col], TB_DEFAULT, TB_DEFAULT);
        }

        if (row == buffer->cursor_y) {
            tb_set_cursor(line->cursor, row);
        }
    } while ((line = line->next) != NULL && ++row);

    tb_present();
}

void ui_destroy()
{
    tb_shutdown();
}
