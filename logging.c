#include "logging.h"

void log_str(const char *msg, ...)
{
    if (logfile == NULL)
        logfile = fopen("logfile.txt", "w");

    va_list args;
    va_start(args, msg);

    vfprintf(logfile, msg, args);

    va_end(args);
}
