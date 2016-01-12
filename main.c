#include <stdio.h>
#include <wchar.h>
#include "gapbuffer.h"

int main(int argc, char *argv[])
{
    struct Gapbuffer buf = new_Gapbuffer("Hello World!", 4);
    rune *out = display(&buf);
    wprintf(L"%ls\n", out);

    move_cursor(&buf, 15);
    delete_backwards(&buf, 1);
    insert_char(&buf, ' ');
    insert_char(&buf, 'D');
    insert_char(&buf, 'r');
    insert_char(&buf, 'a');
    insert_char(&buf, 'g');
    insert_char(&buf, 'o');
    insert_char(&buf, 'n');
    insert_char(&buf, '!');

    out = display(&buf);
    wprintf(L"%ls\n", out);
    
    return 0;
}
