/* This file contains the fuctions that will be used to crate the folowing tables:
1. symbol table
2. macro table
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "globals.h"

/*Max size of a ligal table as a derectives of the memory size*/
#define MAX_TABLE_SIZE 1023



/* The struct of a macro table, an hash table the
keys are the names of the macros
values a list of the lines of the macro


typedef struct macro_table
{
    char *key;
    char *value;
    struct macro_table *next;
} macro_table;
*/


/* A function that will create a new macro table
returns a pointer to the new table

macro_table *init_macro_table(void);

*/

/* A function that will add a new macro to the macro table
returns TRUE if the macro was added successfully
returns FALSE if the macro was not added successfully or the macro already exists in the table

int add_macro(macro_table *table, char *key, char *value);

*/

/* A function that will remove a macro from the macro table and free the memory
returns TRUE if the macro was removed successfully
returns FALSE if the macro was not removed successfully or the macro does not exist in the table

int remove_macro(macro_table *table, char *key);

*/

/* A function that will search for a macro in the macro table and return a pionter to the macro's lines


char *search_macro(macro_table *table, char *name);

*/

/* A function that will free the memory of all the macros in the macro table

void free_macro_table(macro_table *table);

*/

/* A function that will print the macro table (for debugging)

void print_macro_table(macro_table *table);

*/



/* The struct of a symbol table, an hash table the 
keys are the names of the symbols 
values are the addresses of the symbols e.g the line number in the code where the symbol is defined

typedef struct symbol_table
{
    char *key;
    long value;
    } symbol_table;
*/


/* A function that will create a new symbol table
returns a pointer to the new table

symbol_table *init_symbol_table(void);

*/

/* A function that will add a new symbol to the symbol table
returns TRUE if the symbol was added successfully
returns FALSE if the symbol was not added successfully or the symbol already exists in the table

int add_symbol(symbol_table *table, char *key, long value);

*/

/* A function that will remove a symbol from the symbol table and free the memory
returns TRUE if the symbol was removed successfully
returns FALSE if the symbol was not removed successfully or the symbol does not exist in the table

int remove_symbol(symbol_table *table, char *key);

*/

/* A function that will search for a symbol in the symbol table and return a pionter to the symbol's address


symbol_table *search_symbol(symbol_table *table, char *name);

*/

/* A function that will free the memory of all the symbols in the symbol table

void free_symbol_table(symbol_table *table);

*/

/* A function that will print the symbol table (for debugging)

void print_symbol_table(symbol_table *table);

*/




