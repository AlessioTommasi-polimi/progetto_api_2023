cmake_minimum_required(VERSION 3.25)
project(progetto_api_2023 C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(progetto_api_2023
        src/bst_manager.h
        src/globals.h
        src/library.h
        src/list_manager.h
        src/progetto.c
        src/prototype.h
        src/type.h
        src/view.h)
