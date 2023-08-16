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

bool *parse_data(char *rest,data_word *data_image[],long *data_counter,char *error_msg){
    bool *result = TRUE; 
    rest = strtok(rest,",");

    if(strcmp(frase,".data") == 0){ 
        while(strcmp(rest,"\n") != 0){
            int num = atoi(rest);
            if(legal_num(num)){
                strcpy(error_msg,"");                        
            }
            else{
                *result = FALSE;
                strcpy(error_msg,"Error: illegal number\n");
            }
            word *tmp_word = init_data_in_data(num,error_msg);
            data_image[*data_counter] = tmp_word;
            *data_counter++;
            rest = strtok(rest,",");
        }
    }
    else
    {
        while(strcmp(rest,"\n") != 0){
            int i = 0;
            int *val = 0;
            while(rest[i] != '\0'){
                *val = rest[i];
                if(legal_char(rest[i])){
                    strcpy(error_msg,"");                                                    
                }
                else{
                    *result = FALSE;
                    strcpy(error_msg,"Error: illegal char\n");
                    *val = 0;
                }

                word *tmp_word = init_data_in_data(val,error_msg);
                data_image[*data_counter] = tmp_word;
                *data_counter++;
                i++;
            }
            strcpy(error_msg,"");
            if (rest = strtok(rest,",") != NULL){
                *result = FALSE;
                strcpy(error_msg,"Error: illegal string\n");
            }
            word *tmp_word = init_data_in_data(0,error_msg);
            data_image[*data_counter] = tmp_word;
            *data_counter++;
        }
    }
    return result;
}

bool *parse_extern(char *args, symbol_list *symbol_table,char *error_msg){
    bool *result = TRUE;
    char *error;
    char *frase = strtok(args," ");
    if (rest != NULL){
        *result = FALSE;
        printf("Error: .extern should have at least one argument\n");
    }
    while(frase != NULL){
        add_symbol(symbol_table,frase);
        *error = set_symbol_type(symbol_table,frase,EXTERN);
        if(*error != "")
        {
            *result = FALSE;
            printf("Error: unable to add symbol\n");
        }
        frase = strtok(NULL,",");
    }
    return result;
}

int find_instructio(char *op_code char *error_msg){
    int result = 1;/*Helping determine if the instruction is legal / ligal but not all lower case */
    char *error;
    char *lower_op_code = toLowerCase(op_code);
    if(strcmp(op_code,lower_op_code) != 0){
        sprintf(error,"Error: any instruction should be in lower case you wrote %s\n",op_code);
        strcat(error_msg,error);
        result = -1; /*might be ligal but not all lower case*/
    }
    switch (*lower_op_code){
        case "mov":
            if(result == -1){
                return -16;
            }
            return MOV;
        case "cmp":
            return CMP*result;
        case "add":
            return ADD*result;
        case "sub":
            return SUB*result;
        case "lea":
            return LEA*result;
        case "clr": 
            return CLR*result;
        case "not":
            return NOT*result;
        case "inc":
            return INC*result;
        case "dec":
            return DEC*result;
        case "jmp":
            return JMP*result;
        case "bne":
            return BNE*result;
        case "red": 
            return RED*result;
        case "prn":
            return PRN*result;
        case "jsr":
            return JSR*result;
        case "rts":
            return RTS*result;
        case "stop":
            return STOP*result;
    }
    sprintf(error,"Error: %s is not a legal instruction\n",op_code);
    strcpy(error_msg,error);   
    return -30;
}

bool *parse_entry(char *args, symbol_list *symbol_table,char *error_msg){
    bool *result = TRUE;
    char *error;
    char *frase = strtok(args," ");
    if (rest != NULL){
        *result = FALSE;
        printf("Error: .entry should have at least one argument\n");
    }
    while(frase != NULL){
        *error = set_symbol_type(symbol_table,frase,ENTRY);
        if(*error != "")
        {
            *result = FALSE;
            printf("Error: unable to add symbol\n");
        }
        frase = strtok(NULL,",");
    }
    return result;
}

bool *build_ast(char *current_line,symbol_list *symbol_table,long *data_counter,long *instruction_counter,char *error_msg,char *tmp_lable ,data_word *data_image[],
 instruction_word *instruction_image[],char *lable){
    bool *result = TRUE;
    ast_line *ast_line = NULL;
    bool *label_flag;
    int *num_of_words;
    symbol *tmp_symbol;
    label_flag = (lable != NULL)? TRUE:FALSE;
    char *frase = strtok(current_line," ");
    char *rest = strtok(NULL,"\n");/*The line without the first word*/

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
    return result;   
    
    



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
