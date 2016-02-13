#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termbox.h>
#include "tui.h"
#include "python_bindings.h"

#define COLOR_BACKGROUND 18
#define COLOR_FOREGROUND 15

struct Buffer *active_buffer = NULL;

int max(int x, int y) {
    return x < y ? y : x;
}

int min(int x, int y) {
    return x < y ? x : y;
}

int ui_init(struct Buffer *buffer)
{
    active_buffer = buffer;
    return tb_init();
}

void set_view(struct Buffer *buffer)
{
    buffer->view.width = tb_width();
    buffer->view.height = tb_height() - 1;
    buffer->view.start_y = 0;
    buffer->view.start_x = 0;
    buffer->view.status_message = "";
}

void ui_loop()
{
    struct Buffer *buffer = active_buffer;

    set_view(buffer);
    ui_draw(buffer);
    active_buffer = buffer;

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
                //buffer_delete_backwards(buffer, 1);
                python_exec("multi_cursor_backspace()\n");
                break;
            case TB_KEY_SPACE:
                //buffer_insert(buffer, ' ');
                python_exec("multi_cursor_insert(' ')\n");
                break;
            case TB_KEY_ENTER:
                python_exec("newline_and_indent()\n");
                break;
            case TB_KEY_CTRL_S:
                buffer_write_to_file(buffer, buffer->filename);
                break;
            case TB_KEY_CTRL_R:
                python_exec("add_multi_cursor()\n");
                break;
            case TB_KEY_CTRL_W:
                python_exec("reset_multi_cursor()\n");
                break;
            case TB_KEY_CTRL_K:
                python_exec("kill_line()\n");
                break;
            case TB_KEY_CTRL_L:
                python_exec("current_buffer().insert(str(dir()))\n");
                break;
            default:
                if (e.ch > 0 && e.ch <= 128) {
                    tb_utf8_unicode_to_char(&ch, e.ch);
                    //buffer_insert(buffer, ch);
                    char cmd[128];
                    sprintf(cmd, "multi_cursor_insert(\"%c\")", ch);
                    python_exec(cmd);
                }
                break;
            }
            break;
        case TB_EVENT_RESIZE:
            buffer->view.width = tb_width();
            buffer->view.height = tb_height() - 1;
            break;
        default:
            break;
        }
    }
}

void draw_statusbar(struct Buffer *buffer)
{
    int width = tb_width();

    char text[width];
    sprintf(text, "File: %s    %s", buffer->filename,
            buffer->view.status_message);

    int row = buffer->view.height;
    int size = strlen(text);
    char ch;
    for (int i = 0; i < width; ++i) {
        ch = i < size ? text[i] : ' ';
        tb_change_cell(i, row, ch, COLOR_BACKGROUND, 226);
    }
}

void ui_draw(struct Buffer *buffer)
{
    tb_select_output_mode(TB_OUTPUT_256);
    tb_set_clear_attributes(COLOR_FOREGROUND, COLOR_BACKGROUND);
    tb_clear();

    struct Line *line = lines_nth(buffer->lines, buffer->view.start_y);
    int row = 0;
    int start_x = buffer->view.start_x;

    do {
        rune *disp = line_display(line);
        int size = strlen(disp);

        for (int col = 0; col < min(size - start_x, buffer->view.width); ++col) {
            tb_change_cell(col, row, disp[start_x + col], COLOR_FOREGROUND,
                           COLOR_BACKGROUND);
        }

        if (buffer->view.start_y + row == buffer->cursor_y) {
            tb_set_cursor(line->cursor - start_x, row);
        }
    } while ((line = line->next) != NULL && ++row);

    draw_statusbar(buffer);
    tb_present();
}

void ui_destroy()
{
    tb_shutdown();
}
