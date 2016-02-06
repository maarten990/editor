#ifndef PYTHON_BINDINGS_H_TJLINQRT
#define PYTHON_BINDINGS_H_TJLINQRT

#include <Python.h>
#include <structmember.h>
#include <string.h>
#include "buffer.h"
#include "logging.h"

extern struct Buffer *active_buffer;

typedef struct {
    PyObject_HEAD
    struct Buffer *buffer;
} PyBuffer;

static PyMemberDef PyBuffer_members[] = {
    {NULL}  /* Sentinel */
};

static PyObject *PyBuffer_insert(PyBuffer *self, PyObject *args)
{
    char *str;
    int n;

    if (!PyArg_ParseTuple(args, "s:insert", &str)) {
        return NULL;
    }

    n = strlen(str);

    for (int i = 0; i < n; ++i) {
        if (str[i] == '\n')
            buffer_break_at_cursor(self->buffer);
        else
            buffer_insert(self->buffer, str[i]);
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *PyBuffer_cursor(PyBuffer *self)
{
    PyObject *out = PyTuple_New(2);
    PyTuple_SetItem(out, 0, Py_BuildValue("i", self->buffer->cursor_x));
    PyTuple_SetItem(out, 1, Py_BuildValue("i", self->buffer->cursor_y));

    return out;
}

static PyObject *PyBuffer_get_line(PyBuffer *self, PyObject *args)
{
    int y;

    if (!PyArg_ParseTuple(args, "i:insert", &y)) {
        return NULL;
    }

    char *display = line_display(lines_nth(self->buffer->lines, y));
    return Py_BuildValue("s", display);
}

static PyMethodDef PyBuffer_methods[] = {
    {"insert", (PyCFunction)PyBuffer_insert, METH_VARARGS,
     "Insert the given string to the buffer at the cursor location."
    },
    {"cursor", (PyCFunction)PyBuffer_cursor, METH_NOARGS,
     "Return the x and y coordinates of the cursor."
    },
    {"get_line", (PyCFunction)PyBuffer_get_line, METH_VARARGS,
     "Get the contents of a given line."
    },
    {NULL}  /* Sentinel */
};

static PyTypeObject PyBufferType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "editor.Buffer",             /* tp_name */
    sizeof(PyBuffer),            /* tp_basicsize */
    0,                           /* tp_itemsize */
    0,                           /* tp_dealloc */
    0,                           /* tp_print */
    0,                           /* tp_getattr */
    0,                           /* tp_setattr */
    0,                           /* tp_reserved */
    0,                           /* tp_repr */
    0,                           /* tp_as_number */
    0,                           /* tp_as_sequence */
    0,                           /* tp_as_mapping */
    0,                           /* tp_hash  */
    0,                           /* tp_call */
    0,                           /* tp_str */
    0,                           /* tp_getattro */
    0,                           /* tp_setattro */
    0,                           /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,          /* tp_flags */
    "A buffer representation",   /* tp_doc */
    0,                           /* tp_traverse */
    0,                           /* tp_clear */
    0,                           /* tp_richcompare */
    0,                           /* tp_weaklistoffset */
    0,                           /* tp_iter */
    0,                           /* tp_iternext */
    PyBuffer_methods,            /* tp_methods */
    PyBuffer_members,            /* tp_members */
    0,                           /* tp_getset */
    0,                           /* tp_base */
    0,                           /* tp_dict */
    0,                           /* tp_descr_get */
    0,                           /* tp_descr_set */
    0,                           /* tp_dictoffset */
    0,                           /* tp_init */
    0,                           /* tp_alloc */
    0,                           /* tp_new */
};

static PyObject *editor_current_buffer(PyObject *self, PyObject *args)
{
    PyObject *obj = PyObject_CallObject((PyObject *) &PyBufferType, NULL);
    PyBuffer *buf = (PyBuffer *)obj;
    buf->buffer = active_buffer;

    return obj;
}

static PyMethodDef editorMethods[] = {
    {"current_buffer", editor_current_buffer, METH_VARARGS,
     "Get the currently active buffer object."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static PyModuleDef editormodule = {
    PyModuleDef_HEAD_INIT,
    "editor",
    "Module for manipulating the editor.",
    -1,
    editorMethods,
    NULL, NULL, NULL, NULL
};

void python_init();
void python_destroy();
void python_exec(const char *str);
void python_load_plugins();

#endif /* end of include guard: PYTHON_BINDINGS_H_TJLINQRT */
