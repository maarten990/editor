#include <Python.h>
#include "python_bindings.h"
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

    return m;
}

void python_init()
{
    PyImport_AppendInittab("editor", &PyInit_editor);

    Py_Initialize();
    PyObject *mod = PyImport_AddModule("__main__");
    locals = PyModule_GetDict(mod);

    //PyDict_SetItemString(globals, "__builtins__", PyEval_GetBuiltins());
    //PyDict_SetItemString(locals, "__builtins__", PyEval_GetBuiltins());

    python_exec("from editor import *\n");
    python_exec("TESTING = 5\n");
    python_exec("import re\n");
    python_exec("def newline_and_indent():\n"
                "    buffer = current_buffer()\n"
                "    x, y = buffer.cursor()\n"
                "    match = re.search(r'\\w', buffer.get_line(y))\n"
                "    n_spaces = match.start() if match else 0\n"
                "    buffer.insert('\\n' + (' ' * n_spaces))\n"
                "\n");
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
    else {
    }
}
