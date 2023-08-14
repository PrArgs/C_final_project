#include "parser.h"

bool first_parse(char *file_name, symbol_list *symbol_table, long *data_counter, long *instruction_counter){
    FILE *file = fopen(file_name, "r");
    if (file == NULL){
        printf("Error: file %s does not exist\n", file_name);
        return FALSE;
    }
    char *current_line;
    bool result = TRUE;
    char *first_frase;
    char *second_frase;

    /*Iterating over the lines of the file*/
    while (fgets(current_line, MAX_LINE_LENGTH*2, file)){
        first_frase = strtok(current_line, " ");
        second_frase = strtok(NULL, " ");
        if(is_label(first_frase)){
            if(ligal_label(first_frase)){
                if(!(add_symbol(symbol_table, first_frase, instruction_counter))){
                    result = FALSE;
                }
        }
    }

    fclose(file);
    return result;
    
}