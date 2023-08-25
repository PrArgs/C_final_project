#include "table_generator.h"

static int default_hash_function(char *data){
    int arry_size = MEMORY_SIZE;
    int sum = 0;
    int i;
    
    for (i = 0; i < strlen(data); i++)
    {
        sum += (i * (data[i]- '0'));
    }    
    return sum % arry_size;
}



static bool default_compare_function(char *data1, char *data2)
{
    return (strcmp(data1, data2) == 0)? TRUE : FALSE;
}


macro *macro_init(char *name){

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
    int i;
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
    for(i = 0; i < MAX_TABLE_SIZE; i++){
        table->macros_array[i] = NULL;
    }
    return table; 
}

bool add_new_macro(macro_table *macro_table, char *name, char *error_msg){
    int index;
    macro *new_macro;
    macro *current_macro;

    if (table_contains(macro_table, name)){
        sprintf(error_msg,"Error: macro %s already exists\n", name);
        return FALSE;
    }

    index = default_hash_function(name);
    new_macro = macro_init(name);

    if(new_macro == NULL){
        strcpy(error_msg,"Error: memory allocation failed\n");
        return FALSE;
    }    

    if(macro_table->macros_array[index] == NULL)
    {
        macro_table->macros_array[index] = new_macro;
        return TRUE;
    }
    else{
        current_macro = macro_table->macros_array[index];
        while(current_macro->next != NULL){
            current_macro = current_macro->next;
        }
        current_macro->next = new_macro;
        return TRUE;
    }
    free(new_macro);
    return FALSE;
}

bool add_to_macro(macro_table *macro_table, char *data, char *macro_name, char *error_msg){
    node *new_node;
    int index;
    macro *current_macro;

    index = default_hash_function(macro_name);
    current_macro = macro_table->macros_array[index];

    while(current_macro != NULL){
        if(strcmp(current_macro->name, macro_name) == 0){
            new_node = node_init(data);
            add_to_list(current_macro->data, new_node);
            return TRUE;
        }
        current_macro = current_macro->next;
    }
    return FALSE;
}

bool table_contains(macro_table *macro_table, char *macro_name){
    bool result = FALSE;
    int index = default_hash_function(macro_name);
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
    macro *temp, *current_macro;
    int i;

    if(macro_table == NULL){
        return FALSE;
    }
    for(i = 0; i < MAX_TABLE_SIZE; i++){
        current_macro = macro_table->macros_array[i];
        while(current_macro){
            temp = current_macro;
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
    macro *current_macro;
    int index;

    if (!table_contains(macro_table, macro_name)){
        printf("Error: macro %s does not exists\n", macro_name);
        return NULL;
    }
    index = default_hash_function(macro_name);
    current_macro = macro_table->macros_array[index];
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

int get_symbol_val(symbol_list *table, char *symbol_name){
    symbol *current_symbol = table->head;
    while (current_symbol){
        if(strcmp((char *)current_symbol->name, symbol_name) == 0){
            return current_symbol->value;
        }
    }
    return -1;
}

void update_data_symbols(symbol_list *table, int update_value){
    bool result = TRUE;
    symbol *current_symbol = table->head;
    while(current_symbol != NULL){
        if(current_symbol->is_data){
            current_symbol->value = update_value;
        }
        current_symbol = current_symbol->next;
    }
}

bool update_entry_symbols(symbol_list *symbol_table,list **entry_list,int entry_counter,char *error_msg){
    int i;
    bool result = TRUE;
    node *tmp_node;
    char *symbol_name;

    for(i = 0; i < entry_counter; i++){
        tmp_node = get_list_head(entry_list[i]);
        while(tmp_node != NULL){
            symbol_name = get_data(tmp_node);
            if(!(set_symbol_type(symbol_table,symbol_name,ENTRY,error_msg))){
                result = FALSE;
                printf("Error at %d's entry statement: %s\n",i,error_msg);
                strcpy(error_msg,"");
            }
            tmp_node = get_next(tmp_node);
        }        
    }
    return result;
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

symbol *symbol_init(char *name, int value, bool is_data, char *error_massage){
    symbol *symbol;
    ligal_label(name, error_massage);
    symbol = malloc(sizeof(symbol));
    if(symbol == NULL){
        printf("Error: memory allocation failed\n");
        return NULL;
    }
    strcpy((char *)(symbol->name), name);
    symbol->is_entry = FALSE;
    symbol->is_external = FALSE;
    symbol->value = value;
    symbol->is_data = FALSE;
    strcpy((char *)(symbol->error), error_massage);
    return symbol;
}

symbol *get_next_symbol(symbol *symbol){
    return symbol->next;
}


bool set_symbol_type(symbol_list *table,char *symbol_name, symbol_type type, char *result){
    symbol *tmp_symbol = get_symbol(table, symbol_name);

    if(tmp_symbol == NULL){
        sprintf(result, " %s symbol does not exists", symbol_name);
        return FALSE;
    }
    
    switch (type)
    {   case EXT:
            tmp_symbol->is_external = TRUE;
            if(tmp_symbol->is_entry == TRUE){
                sprintf(result, " %s symbol is both external and entry", symbol_name);
                return FALSE;
            }
            break;
        case ENTRY:
            tmp_symbol->is_entry = TRUE;
            if(tmp_symbol->is_external == TRUE){
                sprintf(result, " %s symbol is both external and entry", symbol_name);
                return FALSE;
            }
            break;
        default:
            sprintf(result, " %d is an invalid symbol type", type);
            return FALSE;
            break;
    }
    return TRUE;
}

char *set_symbol_value(symbol_list *table,char *symbol_name, int value, char *error_massage){
    symbol *tmp_symbol;
    tmp_symbol = get_symbol(table, symbol_name);
    strcpy(error_massage, "");
    if(tmp_symbol){
        if(tmp_symbol->value > 0){/*fix symbol check*/
            sprintf(error_massage, " %s symbol already has a value e.g alreay declared", symbol_name);
        }
        else{
            tmp_symbol->value = value;
        }
    }
    else{
        sprintf(error_massage, " %s symbol does not exists", symbol_name);
    }
    return error_massage;
}

void print_symbol(symbol *symbol, char *result)
{ 
    if(symbol != NULL){
        sprintf(result,"|\t%s\t|\t%d\t|",(char*)symbol->name, symbol->value);         
    }
    else{
        printf("Error: symbol is NULL\n");
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

bool add_symbol(symbol_list *table, char *key, int value){
    bool result = TRUE;
    symbol *new_symbol;

    if(search_symbol(table, key) == TRUE){
        printf("Error: symbol %s already exists\n", key);
        return FALSE;
    }

    new_symbol = symbol_init(key, value, FALSE, "");
    if(!new_symbol){
        printf("Error: failed to create new symbol\n");
        return FALSE;
    }

    if(table->head == NULL){
        table->head = new_symbol;
        table->tail = new_symbol;
        if(new_symbol->error != NULL){
            char *result = strcat("Error: ", new_symbol->error);
            result = FALSE;
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
            printf("Error: %s\n", (char *)new_symbol->error);
            result = FALSE;
        }
        return TRUE;       
    }
}

bool remove_symbol(symbol_list *table, char *key){
    symbol *current_symbol = table->head;
    symbol *prev_symbol = NULL;
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

bool search_symbol(symbol_list *table, char *name){
    symbol *current_symbol = table->head;
    while (current_symbol){
        if(strcmp(current_symbol->name, name) == 0){
            return TRUE;
        }
    }
    return FALSE;
}

void free_symbol_list(symbol_list *table){
    symbol *current_symbol = table->head;
    symbol *prev_symbol = NULL;
    while (current_symbol){
        prev_symbol = current_symbol;
        current_symbol = current_symbol->next;
        free(prev_symbol);
    }
    free(table);
}

void print_symbol_list(symbol_list *table){
    char *symbol_buffer;
    symbol *current_symbol = table->head;
    
    symbol_buffer = (char *)malloc(sizeof(char) * MAX_LINE_LENGTH);


    printf("|Symbol name:\t|\t value\t|");
    
    while (current_symbol){
        print_symbol(current_symbol,symbol_buffer);
        printf("%s\n", symbol_buffer);
        current_symbol = current_symbol->next;
    }
    free(symbol_buffer);
}

bool is_entry_s(symbol *symbol){
    return (symbol->is_entry)? TRUE:FALSE;
}

bool is_external_s(symbol *symbol){
    return (symbol->is_external)? TRUE:FALSE;
}



symbol *get_symbol_head(symbol_list *table){
    return table->head;
}

symbol *get_symbol(symbol_list *table, char *symbol_name){
    symbol *current_symbol = table->head;
    while (current_symbol){
        if(strcmp((char *)current_symbol->name, symbol_name) == 0){
            return current_symbol;
        }
    }
    return NULL;
}





