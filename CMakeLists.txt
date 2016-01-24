cmake_minimum_required(VERSION 3.2)
project(Editor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    gapbuffer.c
    gapbuffer.h
    line.c
    line.h
    main.c
    tui.c
    tui.h
    buffer.c
    buffer.h
    logging.h
    logging.c)

add_executable(Editor ${SOURCE_FILES})
target_link_libraries(Editor -ltermbox)