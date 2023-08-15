#include "parser.h"

bool first_parse(char *file_name, symbol_list *symbol_table, long *data_counter, long *instruction_counter){
    FILE *file = fopen(file_name, "r");
    if (file == NULL){
        printf("Error: file %s does not exist\n", file_name);
        return FALSE;
    }
    bool *label_flag = FALSE;
    char *current_line;
    bool result = TRUE;
    char *first_frase;
    char *second_frase;
    char *tmp_lable;
    char *error_msg;
    ast_line *ast_line = NULL;
    data_word *data_image= malloc(sizeof(data_word) * MEMORY_SIZE) = {NULL}};
    instruction_word *instruction_image = malloc(sizeof(instruction_word) * MEMORY_SIZE) = {NULL};


    

    /*Iterating over the lines of the file*/
    while (fgets(current_line, MAX_LINE_LENGTH*2, file)){
        tmp_lable = NULL;
        strcpy(error_msg,"");
        first_frase = strtok(current_line, " ");
        second_frase = strtok(NULL, " ");

        /*First word is a lable*/
        if(is_label(first_frase))
        {
            tmp_lable = strtok(first_frase, ":");
            if(!ligal_label(first_frase)){
                strcat(error_msg,"Error: illegal label name\n");
                result = FALSE;
            }
            if(is_guidance_of_label(second_frase)){
                strcat(error_msg, "Warning: label before .extern or .entry is meaningless\n");
            }
            else if(!(add_symbol(symbol_table, tmp_lable, NOT_REG))){
                result = FALSE;
            }
            current_line = remove_first_word(current_line);        
        }/*Firs word is'nt a lable*/
        ast_line = build_ast(current_line, symbol_table, data_counter, instruction_counter, error_msg, tmp_lable , data_image, instruction_image);
        if(ast_line == NULL){
            result = FALSE;
        }
    }
    if(result){
        if(!second_parse(symbol_table, data_image, instruction_image, data_counter, instruction_counter)){
            result = FALSE;
        }
    }
    fclose(file);
    return result;
    
}