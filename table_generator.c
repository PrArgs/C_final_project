#include "table_generator.h"



macro_table *macro_table_init(){
    macro_table *table = (macro_table *)malloc(sizeof(macro_table));
    if(table == NULL){
        printf("Error: memory allocation failed\n");
        return NULL;
    }
    table->macros_array = (macro **)malloc(sizeof(macro *) * MAX_TABLE_SIZE);
    if(table->macros_array == NULL){
        printf("Error: memory allocation failed\n");
        return NULL;
    }
    /*Init the array to NULL*/
    for(int i = 0; i < MAX_TABLE_SIZE; i++){
        table->macros_array[i] = NULL;
    }
    return table; 
}

bool add_new_macro(macro_table *macro_table, char *name){
    if (table_contains(macro_table, name)){
        printf("Error: macro %s already exists\n", name);
        return FALSE;
    }
    int index = hash_function(name);
    macro *new_macro = (macro *)malloc(sizeof(macro));
    if(new_macro == NULL){
        printf("Error: memory allocation failed\n");
        return FALSE;
    }
    new_macro->name = name;
    new_macro->data = init_list();
    new_macro->next = NULL;

    if(macro_table->macros_array[index] != NULL)
    {
        macro_table->macros_array[index] = &new_macro;
    }
    else{
        macro *current_macro = macro_table->macros_array[index];
        while(current_macro->next != NULL){
            current_macro = current_macro->next;
        }
        current_macro->next = new_macro;
    }
}

bool add_to_macro(macro_table *macro_table, char *data, char *macro_name){
    if (!table_contains(macro_table, macro_name)){
        printf("Error: macro %s does not exists\n", macro_name);
        return FALSE;
    }
    int index = hash_function(macro_name);
    macro *current_macro = macro_table->macros_array[index];
    while(current_macro != NULL){
        if(strcmp(current_macro->name, macro_name) == 0){
            add_to_list(current_macro->data, data);
            return TRUE;
        }
        current_macro = current_macro->next;
    }
    return FALSE;
}

bool remove_macro(macro_table *macro_table, char *data);

/*This is the decleration of function wiil check if a data is in the hash table 
@params macro_table: a pointer to the hash table
@params name: the the name of the macro to check
@returns: 1 if the data is in the hash table and 0 otherwise
*/
bool table_contains(macro_table *macro_table, char *macro_name){
    bool result = FALSE;
    int index = hash_function(macro_name);
    macro *current_macro = macro_table->macros_array[index];
    while(current_macro){
        if(strcmp(current_macro->name, macro_name) == 0){
            result = TRUE;
            break;
        }
        current_macro = current_macro->next;
    }
}


bool free_macro_table(macro_table *macro_table){
    if(macro_table == NULL){
        return FALSE;
    }
    for(int i = 0; i < MAX_TABLE_SIZE; i++){
        macro *current_macro = macro_table->macros_array[i];
        while(current_macro){
            macro *temp = current_macro;
            current_macro = current_macro->next;
            if (!free_list(temp->data)){
                printf("Error: failed to free macro %s\n", temp->name);
                return FALSE;
            }
            free(temp);
        }
    }
    free(macro_table->macros_array);
    free(macro_table);
    return TRUE;
}


/*This function will return a pionter to the list of lines of a macro
@params:
    macro_table: a pointer to the macro table
    macro_name: the name of the macro to get the lines of
*/
list *get_macro_lines(macro_table *macro_table,char *macro_name){
    if (!table_contains(macro_table, macro_name)){
        printf("Error: macro %s does not exists\n", macro_name);
        return NULL;
    }
    int index = hash_function(macro_name);
    macro *current_macro = macro_table->macros_array[index];
    while(current_macro != NULL){
        if(strcmp(current_macro->name, macro_name) == 0){
            return current_macro->data;
        }
        current_macro = current_macro->next;
    }

}

void print_macro(macro *macro){
    printf("Macro name: %s\n", macro->name);
    print_list(macro->data);
}


char *print_macro_table(macro_table *table){
    for (int i = 0; i < MAX_TABLE_SIZE; i++){
        macro *current_macro = table->macros_array[i];
        while(current_macro != NULL){
            print_macro(current_macro);
            current_macro = current_macro->next;
        }
    }
}




symbol_list *init_symbol_list(void){
    printf("init symbol list not implemented\n");
    return NULL;
}


/* A function that will add a new symbol to the symbol table
returns TRUE if the symbol was added successfully
returns FALSE if the symbol was not added successfully or the symbol already exists in the table*/

int add_symbol(symbol_list *table, char *key, long value){
    printf("add symbol not implemented\n");
    return 0;
}


/* A function that will remove a symbol from the symbol table and free the memory
returns TRUE if the symbol was removed successfully
returns FALSE if the symbol was not removed successfully or the symbol does not exist in the table*/

int remove_symbol(symbol_list *table, char *key){
    printf("remove symbol not implemented\n");
    return 0;
}


/* A function that will search for a symbol in the symbol table and return a pionter to the symbol's address*/


symbol_list *search_symbol(symbol_list *table, char *name){
    printf("search symbol not implemented\n");
    return NULL;
    }


/* A function that will free the memory of all the symbols in the symbol table*/

void free_symbol_list(symbol_list *table){
    /*Print not implimnted yet*/
    printf("free_symbol_list not implimnted yet\n");
}

/* A function that will print the symbol table (for debugging)*/

void print_symbol_list(symbol_list *table){
    /*Print not implimnted yet*/
    printf("print_symbol_list not implimnted yet\n");
}



