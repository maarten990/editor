#ifndef EDITOR_LOGGING_H
#define EDITOR_LOGGING_H

#include <stdio.h>
#include <stdarg.h>

FILE *logfile;
void log_str(const char *msg, ...);

#endif //EDITOR_LOGGING_H
