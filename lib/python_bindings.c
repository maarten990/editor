#include <Python.h>
#include <string.h>
#include <stdio.h>
#include "python_mod.h"
#include "python_bindings.h"
#include "tinydir.h"
#include "logging.h"

PyObject *locals;

PyMODINIT_FUNC PyInit_editor()
{
    PyObject *m;

    PyBufferType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PyBufferType) < 0)
        return NULL;

    m = PyModule_Create(&editormodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&PyBufferType);
    PyModule_AddObject(m, "Buffer", (PyObject *)&PyBufferType);

    // add a current_buffer variable
    PyObject *obj = PyObject_CallObject((PyObject *) &PyBufferType, NULL);
    PyBuffer *buf = (PyBuffer *)obj;
    buf->buffer = &active_pane->buf;
    PyModule_AddObject(m, "current_buffer", (PyObject *)buf);

    return m;
}

void python_init()
{
    PyImport_AppendInittab("editor", &PyInit_editor);

    Py_Initialize();
    PyObject *mod = PyImport_AddModule("__main__");
    locals = PyModule_GetDict(mod);

    python_load_plugins();
}

void python_destroy()
{
    //Py_Finalize();
}

void python_exec(const char *str)
{
    PyObject *out = PyRun_String(str, Py_file_input, locals, locals);

    if (out == NULL)
        PyErr_Print();
}

void python_load_plugins()
{
    tinydir_dir dir;
    tinydir_open(&dir, "./plugins/");
    python_exec("import sys; sys.path.append('./plugins/')");

    while (dir.has_next) {
        tinydir_file file;
        tinydir_readfile(&dir, &file);
        if (!file.is_dir) {
            int len = strlen(file.name);
            char name[len];
            char cmd[128];

            strcpy(name, file.name);
            name[strlen(file.name) - 3] = '\0';
            log_str("Importing %s\n", name);
            sprintf(cmd, "from %s import *", name);
            python_exec(cmd);
        }

        tinydir_next(&dir);
    }

    tinydir_close(&dir);
}
