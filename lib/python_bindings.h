#ifndef PYTHON_BINDINGS_H_TJLINQRT
#define PYTHON_BINDINGS_H_TJLINQRT

void python_init();
void python_destroy();
void python_exec(const char *str);
void python_load_plugins();

#endif /* end of include guard: PYTHON_BINDINGS_H_TJLINQRT */
