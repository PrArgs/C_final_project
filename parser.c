#include "parser.h"

bool parse(char *file_name, symbol_list *symbol_table, long *data_counter, long *instruction_counter, data_list *data_image, inst_list *instruction_image){
    
    int line_counter = 0;
    int op_code = -1;
    int second_op_code = -1;
    int entry_counter = 0;
    bool label_flag = FALSE;
    bool result = TRUE;
    bool second_pass = FALSE;
    char *rest;
    char *error_msg;   
    char *first_frase;
    char *current_line;
    char *tmp_lable;    
    list *arg_list;
    list **entry_list;
    FILE *file;
    
    strcat(file_name, ".am");

    file = fopen(file_name, "r");
    if (file == NULL){
        printf("Error: file %s does not exist\n", file_name);
        return FALSE;
    }

    /*Allocate memory to prevet overwriting of data*/
    current_line = (char *)malloc(MAX_POSIBLE_LENGTH);
    error_msg = (char *)malloc(MAX_POSIBLE_LENGTH);
    first_frase = (char *)malloc(MAX_LINE_LENGTH+1);
    rest = (char *)malloc(MAX_LINE_LENGTH+1);
    tmp_lable = (char *)malloc(MAX_LINE_LENGTH+1);
    entry_list = (list **)malloc(1 * sizeof(list *));
    under_construction();

    

    /*Iterating over the lines of the file*/
    while (fgets(current_line,MAX_POSIBLE_LENGTH, file) != NULL){     
        
        strcpy(tmp_lable,"");
        strcpy(error_msg,"");
        label_flag = FALSE;     
        get_first_word(current_line, first_frase);
        remove_first_word(current_line,rest);
        arg_list = list_init();
        
        /*First word is a lable*/
        if(is_label(first_frase)){
            
            label_flag = TRUE;
            strcpy(tmp_lable, strtok(first_frase, ":"));
            get_second_word(current_line, first_frase);
            remove_first_word(current_line, rest);
            remove_first_word(rest, rest);

            if(first_frase == NULL || *first_frase == '\n'){
                
                    printf("ERROR at line %d: lable %s is the only word in the line\n",line_counter, tmp_lable);
                    result = FALSE;
                    line_counter++;
                    list_free(arg_list);
                    continue;
            }
        }

        /*First since all instructions comands are in lower case, if the first word is not in lower case it is not legal*/
        if(strcmp(first_frase,(toLowerCase(first_frase)))){
            result = FALSE;
            printf("ERROR at line %d: %s must be in lower case and might not be ligal \n",line_counter, first_frase);
        }
        
        /*First word turns to lower case*/
        strcpy(first_frase,toLowerCase(first_frase));

        /*Get all the posible arguments*/
        if(!get_args(rest,arg_list, line_counter)){
            result = FALSE;
        }

        if(strcmp(first_frase,".data") == 0 || strcmp(first_frase,".string") == 0){/*if inserting data*/
            if(label_flag){
                if(!(add_symbol(symbol_table, tmp_lable, *data_counter))){
                    result = FALSE;                    
                    line_counter++;
                    list_free(arg_list);
                    continue;
                }/*add the lable to the symbol table or print an error message*/
            }

            if(strcmp(first_frase,".data") == 0){
                if(!parse_data_guid(arg_list, data_image, data_counter, line_counter)){
                    result = FALSE;
                }
            }

            else {
                if(!parse_string_guid(arg_list, data_image, data_counter, line_counter)){
                    result = FALSE;
                }
            }            
            line_counter++;
            list_free(arg_list);
            continue;
        }

        else if (strcmp(first_frase,".extern") == 0 || strcmp(first_frase,".entry") == 0){/*if extern or entry*/

            if(label_flag){
                printf("WARNING at line %d: lable %s is not needed for %s guid and will not show in the symbol table\n",line_counter, tmp_lable, first_frase);
            }

            
            /*.entry will be handled in the second pass*/
            if (strcmp(first_frase,".extern"))
            {
                if(!(parse_extern(arg_list,symbol_table,line_counter, error_msg))){
                    result = FALSE;
                }
            }

            else if(strcmp(first_frase,".entry") == 0){
                if(get_list_head(arg_list) == NULL){
                    printf("ERROR at line %d: .entry must have at least one argument\n",line_counter);
                    result = FALSE;
                }/*if there is no arguments print an error message*/
                else{
                    entry_counter++;
                    entry_list = (list **)realloc(entry_list, entry_counter * sizeof(list *));
                    if(entry_list == NULL){
                        printf("ERROR: could not allocate memory\n");
                        free(current_line);
                        free(error_msg);
                        free(first_frase);
                        free(rest);
                        free(tmp_lable);                            
                        fclose(file);                        
                        exit(1);
                    }
                    entry_list[entry_counter-1] = arg_list;
                }
                
            }
            line_counter++;
            list_free(arg_list);
            continue;
        }        
        
        /*Either an instruction or line or an error*/
        op_code = find_op_code(first_frase);
        if (op_code < 0){
            printf("ERROR at line %d: %s is not a valid instruction\n",line_counter, first_frase);
            result = FALSE;
            line_counter++;
            list_free(arg_list);
            continue;
        }

        if(label_flag){
            
            if(!(add_symbol(symbol_table, tmp_lable, *instruction_counter))){

                result = FALSE;                    
                line_counter++;
                list_free(arg_list);
                continue;
            }/*add the lable to the symbol table or print an error message*/
        }        

        if(!parse_instruction(op_code, arg_list, instruction_image, instruction_counter, line_counter)){
            result = FALSE;
        }
        list_free(arg_list);
        line_counter++;
    }
    if(entry_counter == 0){
        free_entry_list(entry_list, entry_counter);
    }
    

    if(result){ /*start the second pass*/
        update_data_symbols(symbol_table, *instruction_counter);/*updates the data symbols*/
        if(entry_counter > 0){
            if(!update_entry_symbols(symbol_table, entry_list, entry_counter, error_msg)){
                result = FALSE;
            }
            free_entry_list(entry_list, entry_counter);
        }    
            
        rewind(file);
        line_counter = 0;
        second_pass = TRUE;
        op_code = -1;
        second_op_code = -1;

        while (fgets(current_line, MAX_LINE_LENGTH+1, file) != NULL){        
            strcpy(tmp_lable,"");
            strcpy(error_msg,"");
            label_flag = FALSE;     
            get_first_word(current_line, first_frase);
            remove_first_word(current_line,rest);
            arg_list = list_init();
            
            if(is_label(first_frase)){
                remove_first_word(current_line,rest);
                get_second_word(current_line,first_frase);
                remove_first_word(rest,rest);
            }

            if(strcmp(first_frase,".data") == 0 || strcmp(first_frase,".string") == 0 || strcmp(first_frase,".extern") == 0 || (strcmp(first_frase,".entry") == 0)){/*if inserting data*/
                line_counter++;
                list_free(arg_list);
                continue;
            }                    
            else{
                get_args(rest,arg_list, line_counter);
                op_code = find_op_code(first_frase);
                (*instruction_counter)++;/*increase the instruction counter to ignore the op code*/
                second_op_code = lable_op_code(op_code);
                switch (second_op_code)
                {
                case 1:/*check is first arg is a lable*/
                    strcpy(tmp_lable, get_data(get_list_head(arg_list)));
                    if(is_label(tmp_lable)){
                        if(!set_i_inst(instruction_image, *instruction_counter, tmp_lable, symbol_table)){
                            result = FALSE;
                        }
                    }
                    (*instruction_counter)++;
                    if(get_next(get_list_head(arg_list)) != NULL)/*if there is 2 args increase the instruction counter*/
                    {
                        (*instruction_counter)++;
                    }
                    break;

                case 2:
                    (*instruction_counter)++;/*increase the instruction counter to ignore the first arg*/
                    strcpy(tmp_lable, get_data(get_next(get_list_head(arg_list))));
                    if(is_label(tmp_lable)){
                        if(!set_i_inst(instruction_image, *instruction_counter, tmp_lable, symbol_table)){
                            result = FALSE;
                        }
                    }
                    (*instruction_counter)++;
                    break;
                
                case 3:/*Both might be labels*/
                    strcpy(tmp_lable, get_data(get_list_head(arg_list)));
                    if(is_label(tmp_lable)){
                        if(!set_i_inst(instruction_image, *instruction_counter, tmp_lable, symbol_table)){
                            result = FALSE;
                        }
                    }
                    (*instruction_counter)++;

                    strcpy(tmp_lable, get_data(get_next(get_list_head(arg_list))));
                    if(is_label(tmp_lable)){
                        if(!set_i_inst(instruction_image, *instruction_counter, tmp_lable, symbol_table)){
                            result = FALSE;
                        }
                    }
                    (*instruction_counter)++;
                    break;
                
                default:
                    if(get_next(get_list_head(arg_list)) != NULL)/*if there is 2 non label args increase the instruction counter*/
                    {
                        (*instruction_counter)+= 2;
                    }
                    else if (get_list_head(arg_list) != NULL)/*if there is 1 non label arg increase the instruction counter*/
                    {
                        (*instruction_counter)++;
                    }
                    break;
                }
            }
            list_free(arg_list);
            line_counter++;       
        }
    }
    /* Free the allocated memory*/
    free(current_line);
    free(error_msg);
    free(first_frase);
    free(rest);
    free(tmp_lable);
    
          
    fclose(file);
    return result;    
}
