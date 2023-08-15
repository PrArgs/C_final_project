/*
This file is the implementation of the ast.h file
It contains the implementation of all the data structures used in the program
Instruction struct
Gideness struct
Macro struct
*/
#include "ast.h"


bool is_label(char *first_frase){
    char lable_char[] = ":";
    /*Checks the word end with ':'*/
    int lable_len = strlen(first_frase);
    if (first_frase[lable_len-1] != lable_char[0]){
        return FALSE;
    }    
    return TRUE;
}
bool ligal_label(char *first_frase){
    /*too long*/
    if (strlen(first_frase) > MAX_LABEL_LEN){
        return FALSE;
    }
    /*Statrts with a ligal char*/
    else if (first_frase[0] < 'A' || first_frase[0] > 'Z'){
        return FALSE;
    }



ast_line *build_ast(char *current_line,symbol_list *symbol_table,long *data_counter,long *instruction_counter,char *error_msg,char *tmp_lable ,data_word *data_image[],
 instruction_word *instruction_image[],char *lable){
    ast_line *ast_line = NULL;
    bool *label_flag;
    int *num_of_words;
    symbol *tmp_symbol;
    label_flag = (lable != NULL)? TRUE:FALSE;
    char *first_frase = strtok(current_line," ");
    char *rest = strtok(NULL,"\n");
    if ((strcmp(frase,".data") == 0)){
        if(label_flag){
            if(set_symbol_value(symbol_table,lable,data_counter) != ""){
                strcat(error_msg,"Error: unable to add value to symbol\n");
            }
            *data_counter++;


        }

        
    }

 }


bool *is_guidance_of_label(char *frase){
    printf("Not implemented yet\n");
    return FALSE;
}

bool *is_guidnace_of_data(char *frase){
    if ((strcmp(frase,".data") == 0) || (strcmp(frase,".string") == 0)){
        return TRUE;
    }
    return FALSE;
}
    
}
