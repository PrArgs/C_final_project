cmake_minimum_required(VERSION 3.21)
project(C_final_project C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(C_final_project
        assembler.c
        assembler.h
        ast.c
        ast.h
        data_structures.c
        data_structures.h
        file_generator.c
        file_generator.h
        globals.h
        machine_code.c
        machine_code.h
        parser.c
        parser.h
        pre_assembler.c
        pre_assembler.h
        table_generator.c
        table_generator.h
        util.c
        util.h)
