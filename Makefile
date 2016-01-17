CFLAGS = -Wall -pedantic -g -std=c99 -D_XOPEN_SOURCE_EXTENDED
LDFLAGS = -ltermbox

EXEC = editor
SRC = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJECTS = $(SRC:.c=.o)

$(EXEC): $(OBJECTS) $(HEADERS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)
