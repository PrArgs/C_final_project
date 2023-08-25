CC = gcc
+CFLAGS = -g -Wall -ansi -pedantic 

# List of source files
SOURCES = assembler.c data_structures.c ast.c file_generator.c parser.c pre_assembler.c table_generator.c util.c

# List of header files
HEADERS = "globals.h" "data_structures.h" "table_generator.h" "file_generator.h" "pre_assembler.h" "parser.h" "util.h" "ast.h"

# Object files will be derived from source files
OBJECTS = $(SOURCES:.c=.o)

# Build target
assembler: $(OBJECTS)
	$(CC) $(CFLAGS) -o assembler $(OBJECTS)

# Pattern rule to compile .c to .o
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# Clean target
clean:
	rm -f $(OBJECTS) assembler
