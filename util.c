#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "util.h"

int copy_file(const char *from, const char *to)
{
    int src = -1;
    int dest = -1;
    struct stat info;
    int error;

    src = open(from, O_RDONLY);
    error = fstat(src, &info);
    dest = open(to, O_WRONLY | O_CREAT | O_EXCL, info.st_mode);

    if (src < 0 || dest < 0 || error < 0) {
        close(src);
        close(dest);
        return -1;
    }

    char buffer[info.st_blksize];

    int nread;
    while ((nread = read(src, buffer, sizeof(buffer))) > 0) {
        if (write(dest, buffer, nread) < 0) {
            close(src);
            close(dest);
            return -1;
        }
    }

    close(src);
    close(dest);
    return 0;

}
