TEMPLATE = app

CONFIG += release

QMAKE_CFLAGS += -std=gnu99

SOURCES += *.c

# editor core
INCLUDEPATH += -I ..
LIBS += ../editor_core/libeditor_core.a

# termbox
INCLUDEPATH += -I /usr/local/include
LIBS += -L/usr/local/lib -ltermbox

# Python
QMAKE_CFLAGS += $$system(python3-config --cflags)
LIBS += $$system(python3-config --ldflags)
