/* This file contains the fuctions that will be used to crate the folowing tables:
1. symbol table
2. macro table
*/
#ifndef TABLE_GENERATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "data_structures.h"


/*Max size of a ligal table as a derectives of the memory size*/
#define MAX_TABLE_SIZE 1023

/*Define a macro in our assembler
@params:
    name: the name of the macro
    data: the lines of the macro
    next: a pointer to the next macro in the list (for the macro table)
*/
typedef struct macro
{
    char name[MAX_TABLE_SIZE];
    list *data;
    struct macro *next;    
} macro;

/*Define a the macro table as an hash table of macros
@params:
    macros_array: an array of macros lists (so macros with the same hash will be in the same list)
    
*I decided to use the same hashing function for all the possible macro_tables in the code since there is only one macro table 
hance, no need for hash functions param
*/
typedef struct macro_table
{
    macro **macros_array;    
} macro_table;

/*This function will return a pionter to a new macro
@name: the name of the macro
@returns: a pointer to the new macro
*/
macro *macro_init(char *name);

/*This is the decleration of function wiil init a new hash table
@params size: size of the array
@params hash_function: a function that returns the index of the list in which the data should be stored
@params compare_function: a function that compares two data elements and returns 1 if they are equal and 0 otherwise
@returns: a pointer to the new hash table
*/
macro_table *macro_table_init();

/*This is the decleration of function wiil add a new data to the hash table
@params macro_table: a pointer to the macro_table
@params name: the name of the macro to add
@returns: 1 if the data was added successfully and 0 otherwise
*/
bool add_new_macro(macro_table *macro_table, char *name);

/*This function will add new line to a macro in the macro table
@params:
    macro_table: a pointer to the macro table
    data: the data to add
    macro_name: the name of the macro to add the data to
@returns: true if the data was added successfully and false otherwise
*/
bool add_to_macro(macro_table *macro_table, char *data, char *macro_name);

/*This is the decleration of function wiil remove a data from the hash table
@params macro_table: a pointer to the hash table
@params data: the data to remove
@returns: 1 if the data was removed successfully and 0 otherwise
*/
bool remove_macro(macro_table *macro_table, char *data);

/*This is the decleration of function wiil check if a data is in the hash table 
@params macro_table: a pointer to the hash table
@params name: the the name of the macro to check
@returns: 1 if the data is in the hash table and 0 otherwise
*/
bool table_contains(macro_table *macro_table, char *macro_name);

/*This is the decleration of function wiil free the memory of the hash table
@params map: a pointer to the hash table
@returns: true if the data was removed successfully and false otherwise
*/
bool free_macro_table(macro_table *macro_table);

/*This function will return the list of lines of a macro
@params:
    macro_table: a pointer to the macro table
    macro_name: the name of the macro to get the lines of
*/
list *get_macro_lines(macro_table *macro_table,char *macro_name);

/*This function will print a macro
@params:
    macro: a pointer to the macro to print
*/
void print_macro(macro *macro);

/* A function that will return the string of all the macro table (for debugging)*/
char *print_macro_table(macro_table *table);



/*Define a symbol in our assembler
EXTERNAL: a symbol that is defined in another file value 0
ENTRY: a symbol that is defined in the current file value 1
NONE: a symbol that is not defined in any file value 2
*/


typedef enum symbol_type
{
    EXTERNAL=0,
    ENTRY=1,
    
} symbol_type;

/*Define a symbols in our assembler
@params:
    name: the name of the symbol
    value: the value of the symbol (the address in the memory)
    is_external: a boolean that is true if the symbol is external and false otherwise
    is_entry: a boolean that is true if the symbol is entry and false otherwise
    next: a pointer to the next symbol in the list (for the symbol table)
the sybol list will in time be split into two lists, one for external symbols and one for internal symbols in the second pass
*/
typedef struct symbol
{
    char *name;
    int *value;
    bool *is_external;
    bool *is_entry;
    bool *is_data;
    struct symbol_list *next;
    
} symbol;

typedef struct symbol_list
{
    symbol *head;
    symbol  *tail; 
} symbol_list;

/*
___OPTIONAL IF WE DISIDE TO USE HASH TABLES FOR THE SYMBOL TABLE___

typedef struct symbol_list
{
    symbol **symbols_array;    
} symbol_list;
*/
symbol *symbol_init(char *name);

char *set_symbol_type(symbol_list *table,char *symbol_name, symbol_type *type);

char *set_symbol_value(symbol_list *table,char *symbol_name, int *value);

bool *set_symbol_is_data(symbol_list *table,char *symbol_name, bool *is_data);

char *print_symbol(symbol *symbol);

/* A function that will create a new symbol table
returns a pointer to the new table*/
symbol_list *init_symbol_list();

/* A function that will add a new symbol to the symbol table
returns TRUE if the symbol was added successfully
returns FALSE if the symbol was not added successfully or the symbol already exists in the table*/
bool add_symbol(symbol_list *table, char *key, long value);

/* A function that will remove a symbol from the symbol table and free the memory
returns TRUE if the symbol was removed successfully
returns FALSE if the symbol was not removed successfully or the symbol does not exist in the table*/
bool remove_symbol(symbol_list *table, char *key);

/* A function that will search for a symbol in the symbol table and return a pionter to the symbol's address*/
symbol_list *search_symbol(symbol_list *table, char *name);

/* A function that will free the memory of all the symbols in the symbol table*/
void free_symbol_list(symbol_list *table);

/* A function that will print the symbol table (for debugging)*/
void print_symbol_list(symbol_list *table);

/* Hash function that returns the sum of
the multiplication of (the index of the character in the string) and (the ascii value of the character)
modulo the size of the map

@params data: the data to hash

*/
int default_hash_function(char *data)
{
    int arry_size;
    arry_size = MEMORY_SIZE;
    int sum, i;
    sum = 0;
    for (i = 0; i < strlen(data); i++)
    {
        sum += i*data[i];
    }
    return sum%arry_size;
}





/*Our "special" map is a hasj table the data is stored in an array of lists
@params:
    size: size of the array
    array: the array of lists 
    hash_function: a function that returns the index of the list in which the data should be stored
    the default hash function is the sum of the ascii values of the data characters
    compare_function: a function that compares two data elements and returns 1 if they are equal and 0 otherwise
*/

typedef struct map {
    int *size;
    list **array;
    int (*hash_function)(char *,int arry_size);
} map;



bool default_compare_function(char *data1, char *data2)
{
    return strcmp(data1, data2) == 0;
}



#endif


