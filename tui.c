#include <locale.h>
#include <termbox.h>
#include <string.h>
#include "tui.h"
#include "line.h"

void ui_init()
{
    setlocale(LC_ALL, "");
    tb_init();
}

void ui_loop(struct Line *line)
{
    ui_draw(line);

    while (1) {
        ui_draw(line);

        struct tb_event e;
        tb_poll_event(&e);

        char ch;
        switch (e.type) {
            case TB_EVENT_KEY:
                switch(e.key) {
                    case TB_KEY_ESC:
                        return;
                    case TB_KEY_ARROW_LEFT:
                        line_move_cursor(line, -1);
                        break;
                    case TB_KEY_ARROW_RIGHT:
                        line_move_cursor(line, 1);
                        break;
                    case TB_KEY_BACKSPACE:
                    case TB_KEY_BACKSPACE2:
                        line_backspace(line);
                        break;
                    default:
                        if (e.ch > 0 && e.ch <= 128) {
                            tb_utf8_unicode_to_char(&ch, e.ch);
                            line_insert_char(line, ch);
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

void ui_draw(struct Line *line)
{
    tb_clear();

    int row = 0;
    rune *disp = line_display(line);

    tb_select_output_mode(TB_OUTPUT_256);

    for (int col = 0; col < strlen(disp); ++col) {
        tb_change_cell(col, row, disp[col], TB_DEFAULT, TB_DEFAULT);
    }

    tb_set_cursor(line->cursor, 0);

    tb_present();
}

void ui_destroy()
{
    tb_shutdown();
}
