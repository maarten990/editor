#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termbox.h>
#include "tui.h"
#include "python_bindings.h"

#define COLOR_BACKGROUND 18
#define COLOR_FOREGROUND 15

struct list_head buffers;
struct Buffer *active_buffer = NULL;

int max(int x, int y) {
    return x < y ? y : x;
}

int min(int x, int y) {
    return x < y ? x : y;
}

int ui_init()
{
    INIT_LIST_HEAD(&buffers);
    return tb_init();
}

void ui_add_buffer(struct Buffer *buf)
{
    list_add_tail(&buf->list, &buffers);
}

void set_view(struct Buffer *buffer, int offset_x, int offset_y, int width,
              int height, int x, int y)
{
    buffer->view.offset_x = offset_x;
    buffer->view.offset_y = offset_y;
    buffer->view.width = width;
    buffer->view.height = height;
    buffer->view.start_y = y;
    buffer->view.start_x = x;
    buffer->view.status_message = "";
}

void ui_loop()
{
    ui_draw();

    while (1) {
        ui_draw();

        struct tb_event e;
        tb_poll_event(&e);

        char ch;
        switch (e.type) {
        case TB_EVENT_KEY:
            switch(e.key) {
            case TB_KEY_ESC:
                return;
            case TB_KEY_ARROW_LEFT:
                buffer_move_cursor_x(active_buffer, -1);
                break;
            case TB_KEY_ARROW_RIGHT:
                buffer_move_cursor_x(active_buffer, 1);
                break;
            case TB_KEY_ARROW_UP:
                buffer_move_cursor_y(active_buffer, -1);
                break;
            case TB_KEY_ARROW_DOWN:
                buffer_move_cursor_y(active_buffer, 1);
                break;
            case TB_KEY_BACKSPACE:
            case TB_KEY_BACKSPACE2:
                buffer_delete_backwards(active_buffer, 1);
                /* python_exec("multi_cursor_backspace()\n"); */
                break;
            case TB_KEY_SPACE:
                buffer_insert(active_buffer, ' ');
                /* python_exec("multi_cursor_insert(' ')\n"); */
                break;
            case TB_KEY_ENTER:
                buffer_break_at_cursor(active_buffer);
                //python_exec("newline_and_indent()\n");
                break;
            case TB_KEY_CTRL_S:
                buffer_write_to_file(active_buffer, active_buffer->filename);
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
            case TB_KEY_CTRL_N: ; // empty statement
                struct list_head *next = active_buffer->list.next;
                if (next == &buffers)
                    next = next->next;

                active_buffer = list_entry(next, struct Buffer, list);
                break;
            default:
                if (e.ch > 0 && e.ch <= 128) {
                    tb_utf8_unicode_to_char(&ch, e.ch);
                    buffer_insert(active_buffer, ch);
                    /* char cmd[128]; */
                    /* sprintf(cmd, "multi_cursor_insert(\"%c\")", ch); */
                    /* python_exec(cmd); */
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

void draw_statusbar()
{
    int width = tb_width();

    char text[width];
    sprintf(text, "File: %s    %s", active_buffer->filename,
            active_buffer->view.status_message);

    int row = active_buffer->view.height;
    int size = strlen(text);
    char ch;
    for (int i = 0; i < width; ++i) {
        ch = i < size ? text[i] : ' ';
        tb_change_cell(i, row, ch, COLOR_BACKGROUND, 226);
    }
}

void ui_draw()
{
    tb_select_output_mode(TB_OUTPUT_256);
    tb_set_clear_attributes(COLOR_FOREGROUND, COLOR_BACKGROUND);
    tb_clear();

    struct Buffer *buffer;
    list_for_each_entry(buffer, &buffers, list) {
        struct Line *line;
        rune *disp;
        int size;
        int start_x = buffer->view.start_x;
        int row = 0;

        list_for_each_entry(line, &buffer->head.list, list) {
            disp = line_display(line);
            size = strlen(disp);

            int offset_x = buffer->view.offset_x;
            for (int col = offset_x;
                 col < offset_x + min(size - start_x, buffer->view.width);
                 ++col)
            {
                tb_change_cell(col, row, disp[start_x + col - offset_x], COLOR_FOREGROUND,
                               COLOR_BACKGROUND);
            }

            if (buffer == active_buffer &&
                buffer->view.start_y + row == buffer->cursor_y)
            {
                tb_set_cursor(line->cursor - start_x + offset_x, row);
            }

            row += 1;
        }
    }

    draw_statusbar();

    tb_present();
}

void ui_destroy()
{
    tb_shutdown();
}
