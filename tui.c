#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termbox.h>
#include <math.h>
#include "tui.h"
#include "python_bindings.h"

#define COLOR_BACKGROUND 18
#define COLOR_FOREGROUND 15

struct list_head panes;
struct TUI_Pane *active_pane = NULL;

void fill_keymap(struct TUI_Pane *pane)
{
    char **keymap              = pane->keymap;
    keymap[TB_KEY_BACKSPACE]   = "multi_cursor_backspace()\n";
    keymap[TB_KEY_BACKSPACE2]  = "multi_cursor_backspace()\n";
    keymap[TB_KEY_SPACE]       = "multi_cursor_insert(' ')\n";
    keymap[TB_KEY_ENTER]       = "newline_and_indent()\n";
    keymap[TB_KEY_CTRL_R]      = "add_multi_cursor()\n";
    keymap[TB_KEY_CTRL_W]      = "reset_multi_cursor()\n";
    keymap[TB_KEY_CTRL_K]      = "kill_line()\n";
    keymap[TB_KEY_ARROW_RIGHT] = "move_cursor(x=1)\n";
    keymap[TB_KEY_ARROW_LEFT]  = "move_cursor(x=-1)\n";
    keymap[TB_KEY_ARROW_UP]    = "move_cursor(y=-1)\n";
    keymap[TB_KEY_ARROW_DOWN]  = "move_cursor(y=1)\n";
}

int max(int x, int y) {
    return x < y ? y : x;
}

int min(int x, int y) {
    return x < y ? x : y;
}

int ui_init()
{
    INIT_LIST_HEAD(&panes);
    return tb_init();
}

void ui_add_buffer(struct Buffer *buf, int make_active,
                   double width_ratio, double height_ratio,
                   int anchor_x, int anchor_y)
{
    struct TUI_Pane *pane = malloc(sizeof(struct TUI_Pane));
    pane->buf = buf;
    pane->anchor_x = anchor_x;
    pane->anchor_y = anchor_y;
    pane->height_ratio = height_ratio;
    pane->width_ratio = width_ratio;

    set_view(buf, tb_width() * width_ratio, tb_height() * height_ratio,
             0, 0);

    pane->keymap = calloc(pow(2, 16), sizeof(char*));
    fill_keymap(pane);
    
    list_add_tail(&pane->list, &panes);

    if (make_active)
        active_pane = pane;
}

void set_view(struct Buffer *buffer, int width, int height, int x, int y)
{
    buffer->view.width = width;
    buffer->view.height = height;
    buffer->view.start_x = x;
    buffer->view.start_y = y;
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
            if (e.key == TB_KEY_ESC)
                return;

            if (active_pane->keymap[e.key] != NULL) {
                python_exec(active_pane->keymap[e.key]);
            } else {
                if (e.ch > 0 && e.ch <= 128) {
                    tb_utf8_unicode_to_char(&ch, e.ch);
                    char cmd[128];
                    sprintf(cmd, "multi_cursor_insert(\"%c\")", ch);
                    python_exec(cmd);
                }
            }
            break;

        case TB_EVENT_RESIZE: ;
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
    sprintf(text, "File: %s    %s", active_pane->buf->filename,
            active_pane->buf->view.status_message);

    int row = active_pane->buf->view.height;
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

    struct TUI_Pane *pane;
    list_for_each_entry(pane, &panes, list) {
        struct Line *line;
        rune *disp;
        int size;
        int start_x = pane->buf->view.start_x;
        int start_y = pane->buf->view.start_y;
        int row = 0;

        // look up the starting row
        struct list_head *head = &pane->buf->head.list;
        for (int i = 0; i < start_y + 1; ++i)
            head = head->next;

        for (line = list_entry(head, struct Line, list); !line->is_head;
             line = list_entry(line->list.next, struct Line, list))
        {
            disp = line_display(line);
            size = strlen(disp);

            int anchor_x = pane->anchor_x;
            for (int col = anchor_x;
                 col < anchor_x + min(size - start_x, pane->buf->view.width);
                 ++col)
            {
                tb_change_cell(col, row, disp[start_x + col - anchor_x], COLOR_FOREGROUND,
                               COLOR_BACKGROUND);
            }

            if (pane == active_pane &&
                pane->buf->view.start_y + row == pane->buf->cursor_y)
            {
                tb_set_cursor(line->cursor - start_x + anchor_x, row);
            }

            row += 1;

            if (row > pane->buf->view.height)
                break;
        }
    }

    draw_statusbar();

    tb_present();
}

void ui_destroy()
{
    tb_shutdown();
}
