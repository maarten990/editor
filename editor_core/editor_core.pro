TEMPLATE = lib

CONFIG += release staticlib

QMAKE_CFLAGS += -std=gnu99

SOURCES += *.c
HEADERS += *.h

# Python
QMAKE_CFLAGS += $$system(python3-config --cflags)
LIBS += $$system(python3-config --ldflags)
