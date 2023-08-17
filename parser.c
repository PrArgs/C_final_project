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

    
    ast_line *ast_line = NULL;
    int num_of_words;
    symbol *tmp_symbol;
    char *rest;

    

    /*Iterating over the lines of the file*/
    while (fgets(current_line, MAX_LINE_LENGTH+1, file) ){
        tmp_lable = NULL;
        strcpy(error_msg,"");
        first_frase = strtok(current_line, " ");
        second_frase = get_second_word(current_line);
        rest = remove_first_word(current_line);
        

        /*First word is a lable*/
        if(is_label(first_frase))
        {
            if(second_frase == NULL){
                printf("ERROR: lable %s is the only word in the line\n",tmp_lable);
                result = FALSE;
            }
            else{
                toLowerCase(second_frase); /*make sure the second word is lower case so we will be able to compare it*/
                tmp_lable = strtok(first_frase, ":");
                if(tmp_lable == NULL)
                { strcpy(error_msg,"Error: labla is enoty")
                strcpy(tmp_lable,"Default_lable");
                }

                if ((strcmp(second_frase,".extern") == 0) || (strcmp(second_frase,".entry") == 0))
                {          
                    printf("Warning: can't defin lable before extern or entry %s will not be added to symbol table\n",tmp_lable);
                }
                else
                {
                    if((strcmp(second_frase,".data") == 0) || (strcmp(second_frase,".string") == 0))
                    {
                        if(!(add_symbol(symbol_table, tmp_lable, *data_counter,error_msg))){
                            result = FALSE;
                        }
                    }
                    else
                    {
                        
                        if(!(add_symbol(symbol_table, tmp_lable, *instruction_counter,error_msg))){
                            result = FALSE;
                        }
                        
                    }
                }
            }
            current_line = remove_first_word(current_line);
            rest = remove_first_word(current_line);
            first_frase = strtok(current_line, " ");                   
        }

        if ((strcmp(first_frase,".data") == 0) || (strcmp(first_frase,".string") == 0)){
            {           
                if(!(parse_data(rest,data_image,data_counter,error_msg))){
                    result = FALSE;
                }
            }    
        }
        else if (strcmp(frase,".extern"))
        {
             if(!(parse_extern(rest,symbol_table,error_msg))){
                result = FALSE;
             }
        }
        int *instruction = find_instructio(frase , error_msg);
        if (*instruction > -1){
            if(label_flag)
            {
                if(set_symbol_value(symbol_table,lable,instruction_counter) != ""){                
                    strcat(error_msg,"Error: unable to add value to symbol\n");
                    *result = FALSE;
                }
                else{
                    *instruction_counter++;
                    /*Add lable to instruction image*/
                    word *tmp_word = init_lable_in_instruction(lable,error_msg);
                    instruction_image[*instruction_counter] = tmp_word;                
                }
                strcpy(error_msg,"");
            }
            return parse_operation(frase,rest,symbol_table,instruction_counter,error_msg);
        }
    
    }
    if(result){
        if(!second_parse(symbol_table, data_image, instruction_image, data_counter, instruction_counter)){
            result = FALSE;
        }
    }

    if ((strcmp(frase,".data") == 0) || (strcmp(frase,".string") == 0))
    {
        if(label_flag)
        {
            if(set_symbol_value(symbol_table,lable,data_counter) != ""){                
                strcat(error_msg,"Error: unable to add value to symbol\n");
                *result = FALSE;
            }
            else{
                *data_counter++;
                /*Add lable to data image*/
                word *tmp_word = init_lable_in_data(lable,error_msg);
                data_image[*data_counter] = tmp_word;                
            }
            strcpy(error_msg,"");
            return (parse_data(rest,data_image,data_counter,error_msg) && *result);
        }    
    }
    if (strcmp(frase,".extern"))
    {
        return parse_extern(rest,symbol_table,error_msg);
    }
    int *instruction = find_instructio(frase , error_msg);
    if (*instruction > -1){
        if(label_flag)
        {
            if(set_symbol_value(symbol_table,lable,instruction_counter) != ""){                
                strcat(error_msg,"Error: unable to add value to symbol\n");
                *result = FALSE;
            }
            else{
                *instruction_counter++;
                /*Add lable to instruction image*/
                word *tmp_word = init_lable_in_instruction(lable,error_msg);
                instruction_image[*instruction_counter] = tmp_word;                
            }
            strcpy(error_msg,"");
        }
        return parse_operation(frase,rest,symbol_table,instruction_counter,error_msg);
    }
    fclose(file);
    return result;
    
}