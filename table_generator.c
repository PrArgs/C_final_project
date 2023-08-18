#include "table_generator.h"


macro *macro_init(char *name){
int i;
    macro *macro = malloc(sizeof(macro));
    if(macro == NULL){
        printf("Error: memory allocation failed\n");
        return NULL;
    }
    strcpy(macro->name, name);
    macro->data = list_init();
    macro->next = NULL;
    return macro;
}

macro_table *macro_table_init(){
    macro_table *table = (macro_table *)malloc(sizeof(macro_table)); /*TODO mix decleration*/
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
    for(i = 0; i < MAX_TABLE_SIZE; i++){
        table->macros_array[i] = NULL;
    }
    return table; 
}

bool add_new_macro(macro_table *macro_table, char *name){
int index;
    if (table_contains(macro_table, name)){
        printf("Error: macro %s already exists\n", name);
        return FALSE;
    }
    index = default_hash_function(name);
    macro *new_macro = macro_init(name);/*TODO mix decleration*/
    if(new_macro == NULL){
        printf("Error: memory allocation failed\n");
        return FALSE;
    }

    if(macro_table->macros_array[index] != NULL)
    {
        macro_table->macros_array[index] = new_macro;
    }
    else{
        macro *current_macro = macro_table->macros_array[index]; /*TODO mix decleration*/
        while(current_macro->next != NULL){
            current_macro = current_macro->next;
        }
        current_macro->next = new_macro;
    }
return TRUE;
}

bool add_to_macro(macro_table *macro_table, char *data, char *macro_name){
int index;
    if (!table_contains(macro_table, macro_name)){
        printf("Error: macro %s does not exists\n", macro_name);
        return FALSE;
    }
    index = default_hash_function(macro_name);
    macro *current_macro = macro_table->macros_array[index];/*TODO mix decleration*/
    while(current_macro != NULL){
        if(strcmp(current_macro->name, macro_name) == 0){
            node *new_node = node_init(data); /*TODO mix decleration*/
            add_to_list(current_macro->data, new_node);
            return TRUE;
        }
        current_macro = current_macro->next;
    }
    return FALSE;
}

bool remove_macro(macro_table *macro_table, char *data);

bool table_contains(macro_table *macro_table, char *macro_name){
	bool result = FALSE;
	int index;
    index = default_hash_function(macro_name);
    macro *current_macro = macro_table->macros_array[index];
    while(current_macro){
        if(strcmp(current_macro->name, macro_name) == 0){
            result = TRUE;
            break;
        }
        current_macro = current_macro->next;
    }
	return FALSE;
}

bool free_macro_table(macro_table *macro_table){
int i;
    if(macro_table == NULL){
        return FALSE;
    }
    for(i = 0; i < MAX_TABLE_SIZE; i++){
        macro *current_macro = macro_table->macros_array[i];
        while(current_macro){
            macro *temp = current_macro;
            current_macro = current_macro->next;
            if (!list_free(temp->data)){
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

list *get_macro_lines(macro_table *macro_table,char *macro_name){
int index;
    if (!table_contains(macro_table, macro_name)){
        printf("Error: macro %s does not exists\n", macro_name);
        return NULL;
    }
    index = default_hash_function(macro_name);
    macro *current_macro = macro_table->macros_array[index];
    while(current_macro != NULL){
        if(strcmp(current_macro->name, macro_name) == 0){
            return current_macro->data;
        }
        current_macro = current_macro->next;
    }
	
	return NULL;
}

void print_macro(macro *macro){
    printf("Macro name: %s\n", macro->name);
    print_list(macro->data);
}

void print_macro_table(macro_table *table){
int i;
    for (i = 0; i < MAX_TABLE_SIZE; i++){
        macro *current_macro = table->macros_array[i];
        while(current_macro != NULL){
            print_macro(current_macro);
            current_macro = current_macro->next;
        }
    }
}

bool ligal_label(char *label, char *error_massage){
    if(label == NULL){
        return FALSE;
    }
    if(strlen(label) > MAX_LABEL_LENGTH){
        strcat(error_massage,"Error: label is too long\n");
        return FALSE;
    }
    if(isalpha(label[0])){
        strcat(error_massage,"Error: label must start with a letter\n");
        return FALSE;
    }
    return TRUE;
}

symbol *symbol_init(char *name, int *value, bool *is_data, char *error_massage){
    symbol *symbol = malloc(sizeof(symbol));
    ligal_label(name, error_massage);
    if(symbol == NULL){
        printf("Error: memory allocation failed\n");
        return NULL;
    }
    strcpy(symbol->name, name);
    symbol->is_entry = FALSE;
    symbol->is_external = FALSE;
    symbol->value = -1;
    symbol->is_data = FALSE;
    strcpy(symbol->error, error_massage);
    return symbol;
}

char *set_symbol_type(symbol_list *table,char *symbol_name, symbol_type *type){
	int symbol_type;
    symbol *tmp_symbol = get_symbol(table, symbol_name);/*TODO mix decleration problem*/
    char *result = "";
    if(search_symbol(table, symbol_name) == NULL){
        sprintf(result, "Error: %s symbol does not exist", symbol_name);
        return result;
    }
    symbol_type = type;
    switch (symbol_type)
    {   case EXTERNAL:
            tmp_symbol->is_external = TRUE;
            break;
        case ENTRY:
            tmp_symbol->is_entry = TRUE;
            break;
        default:
            sprintf(result, "Error: %d is an invalid symbol type", type);
            break;
    }
    return result;
}

char *set_symbol_value(symbol_list *table,char *symbol_name, int *value){
    symbol *tmp_symbol = get_symbol(table, symbol_name);
    char *result = "";
    if(tmp_symbol){
        if(tmp_symbol->value >0){/*fix symbol check*/
            sprintf(result, "Error: %s symbol already has a value e.g alreay declared", symbol_name);
        }
        else{
            tmp_symbol->value = value;
        }
    }
    else{
        sprintf(result, "Error: %s symbol does not exists", symbol_name);
    }
    return result;
}

char *print_symbol(symbol *symbol)
{
    char *result = "";
    char **string[5] = {" ", " ", " ", " ", " "};
    if(symbol){
        /*Turn each var to string*/
        strcpy(string[0], symbol->name);
        sprintf(string[1], "%ld", symbol->value);
        if(symbol->is_entry){
            strcpy(string[2], "True");
        }
        if(symbol->is_external){
            strcpy(string[3], "True");
        }
        if(symbol->is_data){
            strcpy(string[4], "True");
        }
        /*Concatenate all strings with tabs*/
        result = strcat(result, string[0]);
        result = strcat(result, "\t");
        result = strcat(result, string[1]);
        result = strcat(result, "\t");
        result = strcat(result, string[2]);
        result = strcat(result, "\t");
        result = strcat(result, string[3]);
        result = strcat(result, "\t");
        result = strcat(result, string[4]);
        result = strcat(result, "\t");
        return result;        
    }
    else{
        printf("Error: symbol is NULL\n");
        return NULL;
    }

}

symbol_list *init_symbol_list(void){
    symbol_list *table = malloc(sizeof(symbol_list));
    if(table == NULL){
        printf("Error: memory allocation failed\n");
        exit(1);
    }
    table->head = NULL;
    table->tail = NULL;
    return table;    
}

bool add_symbol(symbol_list *table, char *key,bool *is_data, int *val, char *error_msg){
    bool result = TRUE; /*unused variable*/
    symbol *new_symbol = symbol_init(key, val, is_data, error_msg);/*TODO mix decleration*/
    if(!new_symbol){
        printf("Error: failed to create new symbol\n");
        exit(1);
    }

    if(table->head == NULL){
        table->head = new_symbol;
        table->tail = new_symbol;
        if(new_symbol->error != NULL){
            char *result = strcat("Error: ", new_symbol->error);
            result = FALSE; /* TODO what result does?*/
        }
        return TRUE;
    }
    else{
        if(table->tail->next){/*check if tail next is null mainly for debugging*/
            printf("Error: tail next is not NULL\n");
            return FALSE;
        }
        table->tail->next = new_symbol;
        table->tail = new_symbol;
        if(new_symbol->error != NULL){
            char *result = strcat("Error: ", new_symbol->error); /* TODO what result does?*/
            result = FALSE;
        }
        return TRUE;       
    }
}

bool remove_symbol(symbol_list *table, char *key){
    symbol *current_symbol = table->head; /*TODO decleration problem*/
    symbol *prev_symbol = NULL; /*TODO decleration problem*/
    while (current_symbol){
        if(strcmp(current_symbol->name, key) == 0){
            if(prev_symbol){
                prev_symbol->next = current_symbol->next;
            }
            else{
                table->head = current_symbol->next;
            }
            free(current_symbol);
            return TRUE;
        }
        prev_symbol = current_symbol;
        current_symbol = current_symbol->next;
    }
    return FALSE;
}

symbol/*TODO add _list? or remove from h file*/ *search_symbol(symbol_list *table, char *name){
    symbol *current_symbol = table->head; /*TODO decleration problem*/
    while (current_symbol){
        if(strcmp(current_symbol->name, name) == 0){
            return current_symbol;
        }
    }
    return NULL;
}

void free_symbol_list(symbol_list *table){
    symbol *current_symbol = table->head; /*TODO decleration problem*/
    symbol *prev_symbol = NULL; /*TODO decleration problem*/
    while (current_symbol){
        prev_symbol = current_symbol;
        current_symbol = current_symbol->next;
        free(prev_symbol);
    }
    free(table);
}

void print_symbol_list(symbol_list *table){
    printf("Symbol table:\t|\t value\t|\t is_entry\t|\t is_external\t|\n");
    symbol *current_symbol = table->head; /*TODO decleration problem*/
    while (current_symbol){
        printf("%s\n", print_symbol(current_symbol));
        current_symbol = current_symbol->next;
    }
}



