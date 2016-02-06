CFLAGS = -Wall -pedantic -g -std=c99
LDFLAGS = -ltermbox

# Python
CFLAGS += $(shell python3-config --cflags)
LDFLAGS += $(shell python3-config --ldflags)

EXEC = editor
SRC = $(wildcard *.c)
HEADERS = $(wildcard *.h)
O_FILES = $(SRC:.c=.o)

BUILD_DIR = build
OBJECTS = $(addprefix $(BUILD_DIR)/, $(O_FILES))

$(EXEC): $(OBJECTS) $(HEADERS) $(SRC)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)

.PHONY: clean
