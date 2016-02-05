#include <Python.h>
#include "python_bindings.h"

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

    return m;
}

void python_init()
{
    PyImport_AppendInittab("editor", &PyInit_editor);
    Py_Initialize();
}

void python_destroy()
{
    //Py_Finalize();
}

void python_exec(const char *str)
{
    PyRun_SimpleString(str);
}
