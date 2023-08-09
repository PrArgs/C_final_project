#include "table_generator.h"

/*implementation of table_generator.h*/
macro_table *macro_table_init()


bool add_macro(macro_table *macro_table, macro *data);

/*This is the decleration of function wiil remove a data from the hash table
@params macro_table: a pointer to the hash table
@params data: the data to remove
@returns: 1 if the data was removed successfully and 0 otherwise
*/
bool remove_macro(macro_table *macro_table, char *data);

/*This is the decleration of function wiil check if a data is in the hash table 
@params macro_table: a pointer to the hash table
@params data: the data to check
@returns: 1 if the data is in the hash table and 0 otherwise
*/
bool table_contains(macro_table *macro_table, char *data);

/*This is the decleration of function wiil free the memory of the hash table
@params map: a pointer to the hash table
@returns: true if the data was removed successfully and false otherwise
*/
bool free_macro_table(macro_table *macro_table);

/* A function that will return the string of all the macro table (for debugging)*/

char *print_macro_table(macro_table *table);



/*Define a symbols in our assembler
@params:
    name: the name of the symbol
    value: the value of the symbol (the address in the memory)
    is_external: a boolean that is true if the symbol is external and false otherwise
    next: a pointer to the next symbol in the list (for the symbol table)
the sybol list will in time be split into two lists, one for external symbols and one for internal symbols in the second pass
*/
typedef struct symbol_list
{
    char *name;
    long value;
    bool is_external;
    struct symbol *next;
    
} symbol_list;


/*
___OPTIONAL IF WE DISIDE TO USE HASH TABLES FOR THE SYMBOL TABLE___

typedef struct symbol_table
{
    symbol **symbols_array;    
} symbol_table;
*/

/* A function that will create a new symbol table
returns a pointer to the new table*/

symbol_table *init_symbol_table(void);


/* A function that will add a new symbol to the symbol table
returns TRUE if the symbol was added successfully
returns FALSE if the symbol was not added successfully or the symbol already exists in the table*/

int add_symbol(symbol_table *table, char *key, long value);


/* A function that will remove a symbol from the symbol table and free the memory
returns TRUE if the symbol was removed successfully
returns FALSE if the symbol was not removed successfully or the symbol does not exist in the table*/

int remove_symbol(symbol_table *table, char *key);


/* A function that will search for a symbol in the symbol table and return a pionter to the symbol's address*/


symbol_table *search_symbol(symbol_table *table, char *name);


/* A function that will free the memory of all the symbols in the symbol table*/

void free_symbol_table(symbol_table *table);

/* A function that will print the symbol table (for debugging)*/

void print_symbol_table(symbol_table *table);


/* Hash function that returns the sum of 
the multiplication of (the index of the character in the string) and (the ascii value of the character)
modulo the size of the map

@params data: the data to hash

*/
int default_hash_function(char *data,int arry_size)
{

    if (!arry_size)
    {
        arry_size=MEMORY_SIZE;
    }
    int sum = 0, i=0,u=0;
    for (; i < strlen(data); i++)
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
    if((strcmp(data1, data2) == 0))
    {
        return TRUE;
    }
    return FALSE;
}