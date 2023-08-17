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
}

bool legal_indirect_num(int num){
    if (num > MAX_NUM || num < MIN_NUM){
        return FALSE;
    }
    return TRUE;
}

bool legal_reg(char *reg){    
    if (reg[0] != 'r' || reg[1] < '0' || reg[1] > '7' || reg[2] != '\0'){
        return FALSE;
    }
    return TRUE;
}

bool legal_data_num(int num){
    if (num > MAX_NUM || num < MIN_NUM){
        return FALSE;
    }
    return TRUE;
}

bool legal_char(char c){
    if (c < 0 || c > 127){
        return FALSE;
    }
    return TRUE;
}

word *init_data_in_data(int num,char *error_msg)
{
    char *error;
    int defualt_val = 0;
    if(!legal_data_num(num)){
        sprintf(error,"Error: %d is illegal number\n" , num);
        strcat(error_msg,error);
        num = defualt_val;
    }
    word *result = (word *)malloc(sizeof(word));
    if (result == NULL){
        printf("Error: unable to allocate memory\n");
        exit(1);
    }
    data_word *data_word = (data_word *)malloc(sizeof(data_word));
    if (result == NULL){
        printf("Error: unable to allocate memory\n");
        exit(1);
    }
    data_word->data = num;
    result->data = data_word;
    result->error = error_msg;
    return result;
}

bool loop_over_num(char *args,data_word *data_image[],long *data_counter,char *error_msg){
    bool result = (strcmp(error_msg,"") == 0)? TRUE : FALSE;
    int i = 0;
    int len = strlen(args);
    char *buffer = "";        
    bool dubble_comma = TRUE;/*first arg can't be a comma*/
    bool no_delimiter = FALSE; /*Flag that locates 2 diffrent numbers without a delimiter*/ 

    while(i<len){
        while(args[i] != '/n'){
            strcpy(buffer,"");
            if(args[i] == ' '){
                if (strcmp(buffer,"") != 0){/*ignors befor comma or indicat 2 diffrent numbers without a delimiter*/
                    no_delimiter = TRUE;
                }
                i++;
            }
            else if(args[i] == ',')
            {
                if(dubble_comma){
                    *result = FALSE;
                    strcpy(error_msg,"Error: dubble comma\n");
                    i++;
                }
                else{
                    if(args[i+1] == "\n"){
                        restult = FALSE;
                        strcpy(error_msg,"Error: can't end with a comma\n");
                    }                
                    num = atoi(buffer);
                    strcpy(buffer,"");
                    dubble_comma = TRUE;
                    no_delimiter = FALSE;
                    i++;
                    break;
                }
            }
            /*Checks if char is a digit*/
            else if(args[i] >= '0' && args[i] <= '9'){
                if(no_delimiter){
                    *result = FALSE;
                    strcpy(error_msg,"Error: missing delimiter\n");
                    num = atoi(buffer);
                    strcpy(buffer,"");
                    dubble_comma = FALSE;
                    no_delimiter = FALSE;                    
                    break;
                }
                strcat(buffer,args[i]);
                dubble_comma = FALSE;
            }
            else if(args[i+1] == "\n"){
                if(dubble_comma){
                    printf("Error: can't end with a comma\n");
                    return FALSE;
                }
                else{
                    num = atoi(buffer);
                    i++;
                    break;
                }
            }
            else{
                *result = FALSE;
                strcpy(error_msg,"Error: illegal char\n");
                if(strcmp(buffer,"") != 0){
                    num = atoi(buffer);
                    strcpy(buffer,"");
                    dubble_comma = FALSE;
                    no_delimiter = FALSE;
                    i++;
                    break;
                }                
            }
        }
        word *tmp_word = init_data_in_data(num,error_msg);
        data_image[*data_counter] = tmp_word;
        *data_counter++;
    }    
    return result;
}

bool loop_over_string(char *rest,data_word *data_image[],long *data_counter,char *error_msg){
    bool result = (strcmp(error_msg,"") == 0)? TRUE : FALSE;
    int i = 0;
    int *val = 0;
    while(strcmp(rest,"\n") != 0){
        while(rest[i] != '\0'){
            *val = rest[i]; /*Get the ascii value of the char*/
            if(legal_char(rest[i])){
                strcpy(error_msg,"");                                                    
            }
            else{
                result = FALSE;
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



bool parse_data(char *data_op,char *args,data_word *data_image[],long *data_counter,char *error_msg){    
    if(strcmp(data_op,".data") == 0){
        return loop_over_num(args,data_image,data_counter,error_msg);
    }
    else
    {
        return loop_over_string(args,data_image,data_counter,error_msg);        
    }
}

bool parse_extern(char *args, symbol_list *symbol_table,char *error_msg){
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
    return *result;
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

bool parse_entry(char *args, symbol_list *symbol_table,char *error_msg){
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
/*
bool *build_ast(char *current_line,symbol_list *symbol_table,long *data_counter,long *instruction_counter,char *error_msg,char *tmp_lable ,data_word *data_image[],
 instruction_word *instruction_image[],char *lable){
    bool *result = TRUE;
    ast_line *ast_line = NULL;
    bool *label_flag;
    int *num_of_words;
    symbol *tmp_symbol;
    label_flag = (lable != NULL)? TRUE:FALSE;
    char *frase = strtok(current_line," ");
    char *rest = strtok(NULL,"\n");/*The line without the first word

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
                /*Add lable to data image
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
                /*Add lable to instruction image
                word *tmp_word = init_lable_in_instruction(lable,error_msg);
                instruction_image[*instruction_counter] = tmp_word;                
            }
            strcpy(error_msg,"");
        }
        return parse_operation(frase,rest,symbol_table,instruction_counter,error_msg);
    }
    return result;   
 }
 */


bool is_guidance_of_label(char *frase){
    printf("Not implemented yet\n");
    return FALSE;
}

bool is_guidnace_of_data(char *frase){
    if ((strcmp(frase,".data") == 0) || (strcmp(frase,".string") == 0)){
        return TRUE;
    }
    return FALSE;
}

bool parse_instruction(int ins_code,char *args,instruction_word *instruction_image[],long *instruction_counter,char *error_msg){
    instruction_word *tmp_instruction = malloc(sizeof(instruction_word));
    if(tmp_instruction == NULL){
        strcat(error_msg,"Error: unable to allocate memory\n");
        exit(1);
    }
    bool result = (strcmp(error_msg,"") == 0)? TRUE:FALSE;
    int *word_limit = 0;
    char *error;
    int *ligal_add_source = 0;
    int *ligal_add_dest = 0;
    set_ligal_params(ins_code,ligal_add_source,ligal_add_dest,word_limit);
    switch(*word_limit){
        case 0:
        tmp_instruction->ARE = 0;
        tmp_instruction->dest_add = 0;
        tmp_instruction->op_code = ins_code;
        tmp_instruction->source_add = 0;
        break;

        case 1:
        tmp_instruction->ARE = 0;
        tmp_instruction->dest_add = parse_single_oprand(args,ligal_add_dest,error_msg,instruction_image,instruction_counter);
        tmp_instruction->op_code = ins_code;
        tmp_instruction->source_add = 0;
    }
    return (strcmp(error_msg,"") == 0)? TRUE:FALSE;


    
}

int parse_single_oprand(char *args, int *ligal_add_dest,char *error_msg,instruction_word *instruction_image[],long *instruction_counter){
    char *arg = strtok(args,",");
    int result = 0;
    word *tmp_word = malloc(sizeof(word));
    if(tmp_word == NULL){
        strcat(error_msg,"Error: unable to allocate memory\n");
        exit(1);
    }
    if (arg == NULL){
        strcat(error_msg,"Error: missing argument\n");
        free(tmp_word);
        return -1;
    }
    if(args[0]== '@'){
        if(args[1] == 'r' && args[2] >= '0' && args[2] <= '7'){
            tmp_word->ARE = 0;
            tmp_word->dest_add = 0;
            tmp_word->op_code = 0;
            tmp_word->source_add = args[2] - '0';
            instruction_image[*instruction_counter] = tmp_word;
            *instruction_counter++;
            return 1;
        }
        else{
            strcat(error_msg,"Error: invalid register\n");
            free(tmp_word);
            return -1;
        }
    }
}

void set_ligal_params(int *ins_code, int *ligal_add_source, int *ligal_add_dest, int *word_limit){
    IMMEDIATE=1,
    DIRECT=3,
    REGISTER = 5
    int *all= 
    switch (*ins_code){
        case MOV:
            *word_limit = 2;
            *ligal_add_source = IMMEDIATE+DIRECT+REGISTER;
            *ligal_add_dest = DIRECT+REGISTER;
            break;

        case CMP:
            *word_limit = 2;
            *ligal_add_source = IMMEDIATE+DIRECT+REGISTER;
            *ligal_add_dest = IMMEDIATE+DIRECT+REGISTER;
            break;

        case ADD:
            *word_limit = 2;
            *ligal_add_source = IMMEDIATE+DIRECT+REGISTER;
            *ligal_add_dest = DIRECT+REGISTER;
            break;

        case SUB:
            *word_limit = 2;
            *ligal_add_source = IMMEDIATE+DIRECT+REGISTER;
            *ligal_add_dest = DIRECT+REGISTER;
            break;

        case NOT:
            *word_limit = 1;
            *ligal_add_source = 0;
            *ligal_add_dest = DIRECT+REGISTER;
            break;  

        case CLR:
            *word_limit = 1;
            *ligal_add_source = 0;
            *ligal_add_dest = DIRECT+REGISTER;
            break;

        case LEA:
            *word_limit = 2;
            *ligal_add_source = DIRECT;
            *ligal_add_dest = DIRECT+REGISTER;
            break;
        
        case INC:
            *word_limit = 1;
            *ligal_add_source = 0;
            *ligal_add_dest = DIRECT+REGISTER;
            break;

        case DEC:
            *word_limit = 1;
            *ligal_add_source = 0;
            *ligal_add_dest = DIRECT+REGISTER;
            break;

        case JMP:
            *word_limit = 1;
            *ligal_add_source = 0;
            *ligal_add_dest = DIRECT+REGISTER;
            break;

        case BNE:
            *word_limit = 1;
            *ligal_add_source = 0;
            *ligal_add_dest = DIRECT+REGISTER;
            break;

        case RED:
            *word_limit = 1;
            *ligal_add_source = 0;
            *ligal_add_dest = DIRECT+REGISTER;
            break;

        case RED:
            *word_limit = 1;
            *ligal_add_source = 0;
            *ligal_add_dest = DIRECT+REGISTER;
            break;

        case PRN:
            *word_limit = 1;
            *ligal_add_source = 0;
            *ligal_add_dest = IMMEDIATE+DIRECT+REGISTER;
            break;

        default:
            *word_limit = 0;
    }   
}
    

