#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termbox.h>
#include <Python.h>
#include <math.h>
#include "editor_core/ui.h"
#include "editor_core/python_bindings.h"

#define COLOR_BACKGROUND 18
#define COLOR_FOREGROUND 15

struct list_head panes;
struct UI_Pane *active_pane = NULL;

int max(int x, int y) {
    return x < y ? y : x;
}

int min(int x, int y) {
    return x < y ? x : y;
}

int ui_init()
{
    INIT_LIST_HEAD(&panes);
    int ret = tb_init();
    tb_select_output_mode(TB_OUTPUT_256);
    tb_set_clear_attributes(COLOR_FOREGROUND, COLOR_BACKGROUND);
    tb_clear();

    return ret;
}

void ui_add_buffer(struct Buffer *buf, int make_active,
                   double width_ratio, double height_ratio,
                   int anchor_x, int anchor_y)
{
    struct UI_Pane *pane = malloc(sizeof(struct UI_Pane));
    pane->buf = buf;
    pane->anchor_x = anchor_x;
    pane->anchor_y = anchor_y;
    pane->height_ratio = height_ratio;
    pane->width_ratio = width_ratio;

    set_view(buf, tb_width() * width_ratio, tb_height() * height_ratio,
             0, 0);

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

    if (buffer->view.dirty)
        free(buffer->view.dirty);

    buffer->view.dirty = calloc(height, sizeof(char));
    for (int i = 0; i < buffer->view.height; ++i)
        buffer->view.dirty[i] = 1;
}

void ui_loop()
{
    ui_draw();

    while (1) {
        ui_draw();

        struct tb_event e;
        tb_poll_event(&e);

        switch (e.type) {
        case TB_EVENT_KEY:
            if (e.key == TB_KEY_ESC)
                return;

            // the event keycode is either bound to e.key or e.ch
            int keycode = e.key > 0 ? e.key : e.ch;
            PyObject *key = Py_BuildValue("i", keycode);

            // check if the key is specifically defined in the keymap
            // else check if the keymap has a __missing__ method and call that
            if (active_pane->keymap != NULL &&
                PyDict_Contains(active_pane->keymap, key))
            {
                PyObject *value = PyDict_GetItem(active_pane->keymap, key);
                PyObject *ret = PyObject_CallObject(value, NULL);
                Py_DECREF(ret);
            } else if (PyObject_HasAttrString(active_pane->keymap, "__missing__")) {
                PyObject *func = PyObject_CallMethod(active_pane->keymap,
                                                     "__missing__",
                                                     "i", keycode);
                PyObject *ret = PyObject_CallObject(func, NULL);

                Py_DECREF(func);

                if (ret != NULL)
                    Py_DECREF(ret);
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

void draw_pane(struct UI_Pane *pane)
{
    struct Line *line;
    int start_x = pane->buf->view.start_x;
    int start_y = pane->buf->view.start_y;
    int anchor_x = pane->anchor_x;

    char *contents = gapbuf_display(pane->buf->gapbuf);

    int row = 0;
    int col = 0;
    for (int i = 0; i < pane->buf->gapbuf->bufsize; ++i) {
        if (contents[i] == '\n') {
            col += 1;
            row = 0;
        }

        // TODO: dirty bits
        else {
            tb_change_cell(col, row, contents[i],
                           COLOR_FOREGROUND, COLOR_BACKGROUND);
        }

        if (row > pane->buf->view.height) {
            return;
        }
    }
    // // draw the cursor
    // if (pane == active_pane &&
    //     pane->buf->view.start_y + row == pane->buf->cursor_y)
    // {
    //     tb_set_cursor(line->cursor - start_x + anchor_x, row);
    // }

    // row += 1;
    // if (row > pane->buf->view.height)
    //     break;

    // // reset the dirty bits
    // for (int i = 0; i < pane->buf->view.height; ++i)
    //     pane->buf->view.dirty[i] = 0;
}

void ui_draw()
{
    tb_select_output_mode(TB_OUTPUT_256);
    tb_set_clear_attributes(COLOR_FOREGROUND, COLOR_BACKGROUND);

    struct UI_Pane *pane;
    list_for_each_entry(pane, &panes, list) {
        draw_pane(pane);
    }

    draw_statusbar();
    tb_present();
}

void ui_destroy()
{
    tb_shutdown();
}
