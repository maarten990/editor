CFLAGS = -Wall -Werror -pedantic -std=c99 -Os

EXEC = editor
SRC = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJECTS = $(SRC:.c=.o)

$(EXEC): $(OBJECTS) $(HEADERS)
	$(CC) $(OBJECTS) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)
