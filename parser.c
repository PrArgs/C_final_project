#include "parser.h"

bool parse(char *file_name, symbol_list *symbol_table, long *data_counter, long *instruction_counter, data_list *data_image, inst_list *instruction_image){
    FILE *file = fopen(file_name, "r");
    if (file == NULL){
        printf("Error: file %s does not exist\n", file_name);
        return FALSE;
    }
    int zero = 0;
    int line_counter = 0;
    bool label_flag;
    char *current_line;
    bool *result = TRUE;
    char *first_frase;
    char *second_frase;
    char tmp_lable[MAX_LABEL_LENGTH+1];
    char *error_msg;


    
    
    int num_of_words;
    symbol tmp_symbol[MAX_LABEL_LENGTH+1];
    char *rest;
    int op_code = -1;
    bool second_pass = FALSE;
    list *arg_list;    

    

    /*Iterating over the lines of the file*/
    while (fgets(current_line,MAX_POSIBLE_LENGTH, file) != NULL){        
        strcpy(tmp_lable,"");
        strcpy(error_msg,"");
        label_flag = FALSE;     
        first_frase = strtok(current_line, " ");
        strcpy(rest,remove_first_word(current_line));
        arg_list = list_init();
        
        

        /*First word is a lable*/
        if(is_label(first_frase))
        {
            label_flag = TRUE;
            strcpy(tmp_lable, strtok(first_frase, ":"));
            first_frase = get_second_word(current_line);
            current_line = remove_first_word(current_line);
            strcpy(rest,remove_first_word(current_line));

            if(first_frase == NULL || first_frase == '\n'){
                
                    printf("ERROR at line %d: lable %s is the only word in the line\n",line_counter, tmp_lable);
                    result = FALSE;
                    line_counter++;
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
                if(!(add_symbol(symbol_table, tmp_lable, data_counter))){
                    result = FALSE;                    
                    line_counter++;
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
            continue;
        }

        else if (strcmp(first_frase,".extern") == 0 || strcmp(first_frase,".entry") == 0){/*if extern or entry*/

            if(label_flag){
                printf("WARNING at line %d: lable %s is not needed for %s guid and will not show in the symbol table\n",line_counter, tmp_lable, first_frase);
            }

            
            /*.entry will be handled in the second pass*/
            if (strcmp(first_frase,".extern"))
            {
                if(!(parse_extern(arg_list,symbol_table,line_counter))){
                    result = FALSE;
                }
            }
            line_counter++;
            continue;
        }        
        
        /*Either an instruction or line or an error*/
        op_code = find_op_code(first_frase);
        if (op_code < 0){
            printf("ERROR at line %d: %s is not a valid instruction\n",line_counter, first_frase);
            result = FALSE;
            line_counter++;
            continue;
        }

        if(label_flag){
            if(!(add_symbol(symbol_table, tmp_lable, instruction_counter))){
                result = FALSE;                    
                line_counter++;
                continue;
            }/*add the lable to the symbol table or print an error message*/
        }        

        if(!parse_instruction(op_code, arg_list, instruction_image, instruction_counter, line_counter,second_pass)){
            result = FALSE;
        }
        line_counter++;
        list_free(arg_list);   
    }

    if(result){ /*start the second pass*/
        update_data_symbols(symbol_table, data_counter);/*updates the data symbols*/
        rewind(file);
        line_counter = 0;
        second_pass = TRUE;

        while (fgets(current_line, MAX_LINE_LENGTH+1, file) != NULL){        
            strcpy(tmp_lable,"");
            strcpy(error_msg,"");
            label_flag = FALSE;     
            strcpy(first_frase, strtok(current_line, " "));
            strcpy(rest,remove_first_word(current_line));
            
            if(is_label(first_frase)){
                first_frase = get_second_word(current_line);
                strcpy(rest,remove_first_word(remove_first_word(current_line)));
            }

            /*First word mend for parsing*/
            if(strcmp(first_frase,(toLowerCase(first_frase)))){
                result = FALSE;
                printf("ERROR at line %d: %s must be in lower case and might not be ligal \n",line_counter, first_frase);
            }

            /*First word turns to lower case*/
            strcpy(first_frase,toLowerCase(first_frase));

            if(strcmp(first_frase,".entry") == 0){/*if entry*/
                if(!parse_entry(arg_list,symbol_table,line_counter,error_msg)){
                    result = FALSE;
                }
            }
            else if(strcmp(first_frase,".data") == 0 || strcmp(first_frase,".string") == 0 || strcmp(first_frase,".extern") == 0){/*if inserting data*/
                line_counter++;
                continue;
            }

            else{/*instruction*/
                op_code = find_op_code(first_frase);
                
                get_args(rest,arg_list, line_counter);
                if(!parse_instruction(op_code, arg_list, instruction_image, instruction_counter, line_counter,second_pass)){
                    result = FALSE;
                }
            }
            line_counter++;       
        }
    }      
    fclose(file);
    return result;    
}
