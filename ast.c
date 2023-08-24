/*
This file is the implementation of the ast.h file
It contains the implementation of all the data structures used in the program
Instruction struct
Gideness struct
Macro struct
*/
#include "ast.h"

bool valid_addressing(int given_addressing, int ligal_addressing);
int parse_single_oprand(char *args,char *error_msg,instruction_word *tmp_word );
void remove_last_inst(inst_list *instruction_image);
void combine_register(instruction_word *first_rand, instruction_word *second_rand);
bool ligal_label(char *first_frase){
    /*too long*/
    if (strlen(first_frase) > MAX_LABEL_LENGTH){
        return FALSE;
    }
    /*Statrts with a ligal char*/
    else if (first_frase[0] < 'A' || first_frase[0] > 'Z'){
        return FALSE;
    }
}

bool legal_indirect_num(int num){
    if (num > MAX_IMMEDIATE_VALUE || num < MIN_IMMEDIATE_VALUE){
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
    if (num > MAX_DATA_VALUE || num < MIN_DATA_VALUE){
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

bool init_data_in_data(data_list *data_image,long *data_counter,int num,int line_counter){
    bool error = TRUE;
    data_word *d_word;
    data_node *result;
    int defualt_val;

    defualt_val = 0;
    if(!legal_data_num(num)){
        printf("Error at line %d: %d is not a legal number\n",line_counter,num);
        num = defualt_val;
    }
    d_word =  (data_word *)malloc(sizeof(data_word));
    if (d_word == NULL){
        printf("Error: unable to allocate memory\n");
        return FALSE;
    }
    if(num < 0){
        num = complement_2(num);
    }

    d_word->data = (unsigned int)num;
    error = add_to_data_list(data_image,d_word);
    (*data_counter)++;
    return error;

}

void set_ligal_params(int ins_code, int *ligal_add_source, int *ligal_add_dest, int *word_limit){
    int w_limit=0, l_add_source=0, l_add_dest=0;
    switch (ins_code){
        case MOV:
            w_limit = 2;
            l_add_source = IMMEDIATE+DIRECT+REGISTER;
            l_add_dest = DIRECT+REGISTER;
            break;

        case CMP:
            w_limit = 2;
            l_add_source = IMMEDIATE+DIRECT+REGISTER;
            l_add_dest = IMMEDIATE+DIRECT+REGISTER;
            break;

        case ADD:
            w_limit = 2;
            l_add_source = IMMEDIATE+DIRECT+REGISTER;
            l_add_dest = DIRECT+REGISTER;
            break;

        case SUB:
            w_limit = 2;
            l_add_source = IMMEDIATE+DIRECT+REGISTER;
            l_add_dest = DIRECT+REGISTER;
            break;

        case NOT:
            w_limit = 1;
            l_add_source = 0;
            l_add_dest = DIRECT+REGISTER;
            break;  

        case CLR:
            w_limit = 1;
            l_add_source = 0;
            l_add_dest = DIRECT+REGISTER;
            break;

        case LEA:
            w_limit = 2;
            l_add_source = DIRECT;
            l_add_dest = DIRECT+REGISTER;
            break;
        
        case INC:
            w_limit = 1;
            l_add_source = 0;
            l_add_dest = DIRECT+REGISTER;
            break;

        case DEC:
            w_limit = 1;
            l_add_source = 0;
            l_add_dest = DIRECT+REGISTER;
            break;

        case JMP:
            w_limit = 1;
            l_add_source = 0;
            l_add_dest = DIRECT+REGISTER;
            break;

        case BNE:
            w_limit = 1;
            l_add_source = 0;
            l_add_dest = DIRECT+REGISTER;
            break;

        case RED:
            w_limit = 1;
            l_add_source = 0;
            l_add_dest = DIRECT+REGISTER;
            break;

        case JSR:
            w_limit = 1;
            l_add_source = 0;
            l_add_dest = DIRECT+REGISTER;
            break;

        case PRN:
            w_limit = 1;
            l_add_source = 0;
            l_add_dest = IMMEDIATE+DIRECT+REGISTER;
            break;

        default:
            w_limit = 0;
            l_add_source = 0;
            l_add_dest = 0;
    }
    word_limit = &w_limit;
    ligal_add_source = &l_add_source;
    ligal_add_dest = &l_add_dest;   
}

bool valid_addressing(int given_addressing, int ligal_addressing)
{
    switch (ligal_addressing)
    {/*Since the ligal addresings are 1,3,5 || 3,5 || 3, we and the sum of each is unique, we can didacte the ligal addressing by the sum of the ligal addressing*/
    case 9:
        if(given_addressing == 1 || given_addressing == 3 || given_addressing == 5)
            return TRUE;
        else
            return FALSE;
        break;

    case 8:
        if(given_addressing == 3 || given_addressing == 5)
            return TRUE;
        else
            return FALSE;
        break;

    case 3:
        if(given_addressing == 3)
            return TRUE;
        else
            return FALSE;
        break;
    
    default:/*For debuging*/
        printf("Something is wrong at valid_addressing function given %d and %d\n",given_addressing,ligal_addressing);
        return FALSE;
        break;
    }
}


void combine_register(instruction_word *first_rand, instruction_word *second_rand){    
    ((second_rand->word_type.reg_word.source_reg)) =(unsigned int)((first_rand->word_type.reg_word.dest_reg));
     
}
/*
##########################
Non encapsulated functions
##########################
*/

bool get_args(char *args, list *arg_array, int line_counter){

    int quote_counter = 0;
    char *buffer;/*Too long arguments will be checked by context*/
    char single_char; 
    bool reading_arg = FALSE, no_delimiter = FALSE, last_comma = FALSE, result = TRUE;
    node *tmp_node = NULL;
    buffer = (char *)malloc(sizeof(char)*(MAX_LINE_LENGTH+1));

    while(*args != '\n'){
        single_char = *args;
        if(single_char == ','){
            if(reading_arg && quote_counter == 0){
                reading_arg = FALSE;/*End of reading arg*/
                no_delimiter = FALSE;
                tmp_node = node_init(buffer);
                if(tmp_node == NULL){
                    printf("Error at line %d: unable to allocate memory\n",line_counter);
                    result = FALSE;
                }                
                add_to_list(arg_array,tmp_node);
                strcpy(buffer,"");
                tmp_node = NULL;
            }
            else if(quote_counter){
                if(quote_counter >1){
                    result = FALSE;
                    printf("Error at line %d: string can't have more than 1 argument %s will not be added to data image\n",line_counter,args);
                }
                else{
                    strncat(buffer, &single_char, 1);
                }

            }
            else
            {
                printf("Error at line %d: comma must come after an argument\n",line_counter);
                result = FALSE;
            }
            last_comma = TRUE;
        }

        else if (isspace((unsigned char)single_char)){/*Handle white spaces*/
            if(reading_arg && quote_counter == 0)
            {
                if(strcmp(buffer,"-") == 0 || strcmp(buffer,"+") == 0){/*if sign is not followed by a number*/
                    printf("Error at line %d: %s must be followed by a number\n",line_counter,buffer);                   
                }                
                no_delimiter = TRUE;
            }
            else if(quote_counter<2){
                strncat(buffer, &single_char, 1);
            }
        }/*End of white space*/

        else if(single_char == '\"'){
            if(quote_counter > 1){
                printf("Error at line %d: string can't have more than 1 argument %s will not be added to data image\n",line_counter,args);
                free(buffer);
                return FALSE;
            }
            else if (quote_counter){/*closing quote*/
                reading_arg = FALSE;
                no_delimiter = FALSE;
                tmp_node = node_init(buffer);
                if(tmp_node == NULL){
                    printf("Error at line %d: unable to allocate memory\n",line_counter);
                    result = FALSE;
                }
                add_to_list(arg_array,tmp_node);
                strcpy(buffer,"");
                tmp_node = NULL;
            } 
            else{/*opening quote*/
                reading_arg = TRUE;
                no_delimiter = TRUE;
            }
            quote_counter++;
        }

        else{/*We have a char*/                    
            if(no_delimiter && quote_counter == 0){
                printf("Error at line %d: %s must be followed by a delimiter\n",line_counter,buffer);
                result = FALSE;
                tmp_node = node_init(buffer);
                if(tmp_node == NULL){
                    printf("Error at line %d: unable to allocate memory\n",line_counter);
                    result = FALSE;
                }
                add_to_list(arg_array,tmp_node);
                strcpy(buffer,"");
                tmp_node = NULL;
                strncat(buffer, &single_char, 1);
            }

            else if(quote_counter > 1){
                printf("Error at line %d: string can't have more than 1 argument %s will not be added to data image\n",line_counter,args);
                free(buffer);
                return FALSE;
            }

            else{
                last_comma = FALSE;
                reading_arg = TRUE;
                strncat(buffer, &single_char, 1);
            } 
        }
        args++;        
    }

    if (reading_arg){
        tmp_node = node_init(buffer);
        if(tmp_node == NULL){
            printf("Error at line %d: unable to allocate memory\n",line_counter);
            result = FALSE;
        }
        add_to_list(arg_array,tmp_node);
        strcpy(buffer,"");
        tmp_node = NULL;
    }
    else if(last_comma){
        printf("Error at line %d: line can't end with a comma\n",line_counter);
        result = FALSE;
    }
    free(buffer);
    return result;
}

bool is_label(char *first_frase){
    char lable_char = ':';
    /*Checks the word end with ':'*/
    int lable_len;
    lable_len = strlen(first_frase);
    if (first_frase[lable_len-1] != lable_char){
        return FALSE;
    }    
    return TRUE;
}

bool parse_data_guid(list *args,data_list *data_image,long *data_counter,int line_counter)
{
    bool result = TRUE;
    int num = 0;
    node *tmp_node;
    tmp_node = get_list_head(args);

    if(tmp_node == NULL){
        printf("Error at line %d: .data command must have at least one argument\n",line_counter);
        return FALSE;
    }

    while(tmp_node != NULL){                                     
        num = atoi(get_data(tmp_node));
        if(!(init_data_in_data(data_image,data_counter,num,line_counter))){
            result = FALSE;
        }
        tmp_node = get_next(tmp_node);         
    }

    return result;
}

bool parse_string_guid(list *args,data_list *data_image,long *data_counter,int line_counter){

    bool result = TRUE;
    int num = 0;
    node *tmp_node;
    char *buffer;
    tmp_node = get_list_head(args);

    if(tmp_node == NULL){
        printf("Error at line %d: .data command must have at least one argument\n",line_counter);
        return FALSE;
    }

    if(get_next(tmp_node) != NULL){
        printf("Error at line %d: .string command must have only one argument\n",line_counter);
        return FALSE;
    }

    buffer = get_data(tmp_node);


    while(*buffer != '\0'){                                    
        num =(unsigned int)(*buffer - '0');
        if(!(init_data_in_data(data_image,data_counter,num,line_counter))){
            result = FALSE;
        }        
        /*Move to next char*/
        buffer++;         
    }
    num =0;
    if(!(init_data_in_data(data_image,data_counter,num,line_counter))){
        result = FALSE;
    }  
    return result;
}

bool parse_extern(list *args, symbol_list *symbol_table,int line_counter){
    
    node *tmp_node;
    bool result = TRUE;
    char *error;
    char *buffer;
    tmp_node = get_list_head(args);


    if (tmp_node == NULL){
        printf("Error at line %d: .extern command must have at least one argument\n",line_counter);
        return FALSE;
    }
    
    
    error = (char *)malloc(sizeof(char)*(MAX_LINE_LENGTH+1));

    while(tmp_node != NULL){
        buffer = get_data(tmp_node);
        if(!ligal_label(buffer)){
            result = FALSE;
            printf("Error at line %d: %s is not a legal label and wount be added to the symbol list",line_counter,buffer);    
        }
        else{            
            if(!(add_symbol(symbol_table,buffer,0))){
                result = FALSE;                    
                printf("Error at line %d: %s is not a legal label and wount be added to the symbol list",line_counter,buffer);
            }
            else{
                strcpy(error,set_symbol_type(symbol_table,buffer,EXT));
                if(strcmp(error,"") != 0){
                    result = FALSE;
                    printf("Error at line %d: %s\n",line_counter,error);
                    strcpy(error,"");
                }
            }
        }
        tmp_node = get_next(tmp_node);
    }
    free(error) ;
    return result;
}

int find_op_code(char *op_code) {
    if (strcmp(op_code, "mov") == 0) {
        return MOV;
    } else if (strcmp(op_code, "cmp") == 0) {
        return CMP;
    } else if (strcmp(op_code, "add") == 0) {
        return ADD;
    } else if (strcmp(op_code, "sub") == 0) {
        return SUB;
    } else if (strcmp(op_code, "lea") == 0) {
        return LEA;
    } else if (strcmp(op_code, "clr") == 0) {
        return CLR;
    } else if (strcmp(op_code, "not") == 0) {
        return NOT;
    } else if (strcmp(op_code, "inc") == 0) {
        return INC;
    } else if (strcmp(op_code, "dec") == 0) {
        return DEC;
    } else if (strcmp(op_code, "jmp") == 0) {
        return JMP;
    } else if (strcmp(op_code, "bne") == 0) {
        return BNE;
    } else if (strcmp(op_code, "red") == 0) {
        return RED;
    } else if (strcmp(op_code, "prn") == 0) {
        return PRN;
    } else if (strcmp(op_code, "jsr") == 0) {
        return JSR;
    } else if (strcmp(op_code, "rts") == 0) {
        return RTS;
    } else if (strcmp(op_code, "stop") == 0) {
        return STOP;
    }
    return -1;
}

bool parse_entry(list *args, symbol_list *symbol_table,char *error_msg,int line_counter){
    bool result = TRUE;
    char error_buffer[MAX_LINE_LENGTH];
    char *error;    
    node *tmp_node;

    error = (char *)malloc(sizeof(char)*(MAX_LINE_LENGTH+1));
    tmp_node = get_list_head(args);

    if (tmp_node != NULL){
        result = FALSE;
        printf("Error: .entry should have at least one argument\n");
    }  

    while(tmp_node != NULL){
        strcpy(error,set_symbol_type(symbol_table,get_data(tmp_node),ENTRY));
        if(strcmp(error,"") != 0){
            result = FALSE;
            printf("Error at line %d: %s\n",line_counter,error);
            strcpy(error,"");
        }
        tmp_node = get_next(tmp_node);
    }
    return result;
}

int parse_single_oprand(char *args,char *error_msg,instruction_word *tmp_word ){
    int result = 0;
    int num = 0;    
    char error_format[MAX_LINE_LENGTH];
    immediate_direct_word *rand_word;
    register_word *reg_word;    

    if (args == NULL){
        strcat(error_msg,"Error: missing argument\n");
        free(tmp_word);

        return -1;
    }

    if(isalpha(args[0])){/*Arg is a label*/
            rand_word = (immediate_direct_word*)malloc(sizeof(immediate_direct_word));
            if(rand_word == NULL){
                strcat(error_msg,"Error: unable to allocate memory\n");
                return 0;
            }
            rand_word->ARE = 0;
            rand_word->operand = 0;

            tmp_word = (instruction_word *)rand_word;

            return IMMEDIATE;
    }
    
    else if(args[0]== '@'){/*Arg is a sespected */
        if(args[1] == 'r' && args[2] >= '0' && args[2] <= '7' && args[3] == '\0'){
            reg_word = (register_word*)malloc(sizeof(register_word));
            if(reg_word == NULL){
                strcat(error_msg,"Error: unable to allocate memory\n");
                return 0;
            }
            reg_word->ARE = 0;
            reg_word->source_reg = 0;
            reg_word->dest_reg = (unsigned int)(args[2] - '0');

            tmp_word = (instruction_word *)rand_word;
            return REGISTER;
            }

        else{/*Invalid register won't be added to image*/            
            sprintf(error_msg," %s is invalid register\n",args);
            return 0;
        }
    }

    else{
        num = atoi(args);
        if(!legal_indirect_num(num)){
            sprintf(error_msg," %s is invalid immediate value\n",args);
            return 0;
        }
        else{
            rand_word = (immediate_direct_word*)malloc(sizeof(immediate_direct_word));
            if(rand_word == NULL){
                strcat(error_msg,"Error: unable to allocate memory\n");
                free(tmp_word);
                return 0;
            }
            if (num < 0 )
            {
                num = (unsigned int)complement_2(num);
            }

            rand_word->ARE = 0;
            rand_word->operand = (unsigned int)num;
            
            tmp_word = (instruction_word *)rand_word;
            return DIRECT;            
        }
    }
}

bool parse_instruction(int ins_code, list *args, inst_list *instruction_image,long *instruction_counter,int line_counter,bool second_pass){
    
    int i = 0, f_rand_add = 0, s_rand_add = 0;       
    int *word_limit, *ligal_add_source, *ligal_add_dest;
    node *tmp_node;
    char *error;        
    char *args_array[]= {NULL,NULL};
    bool result = TRUE;

    instruction_word *rapping_word=NULL;
    instruction_word *first_rand= NULL;
    instruction_word *second_rand= NULL;
    instruction_op_word *tmp_instruction = NULL;
      
    tmp_node = get_list_head(args);
    while (tmp_node != NULL)
    {
        if(i >1){
            printf("Error at line %d: too many arguments\n",line_counter);
            result = FALSE;
            break;
        }
        strcpy(args_array[i],get_data(tmp_node));
        tmp_node = get_next(tmp_node);
        i++;
    }  

    if(second_pass){
        return update_lable_rand(args_array,instruction_counter,instruction_image);
    }

    else{
        error = (char *)malloc(sizeof(char)*(MAX_LINE_LENGTH+1)); 
        args_array[0] = (char *)malloc(sizeof(char)*(MAX_LINE_LENGTH+1));
        args_array[1] = (char *)malloc(sizeof(char)*(MAX_LINE_LENGTH+1)); 
        tmp_instruction = (instruction_op_word *)malloc(sizeof(instruction_op_word));

        if(tmp_instruction == NULL){
            printf("Error at line %d: unable to allocate memory\n",line_counter);
            return FALSE;
        } 
        
        
        set_ligal_params(ins_code,ligal_add_source,ligal_add_dest,word_limit);

        switch(*word_limit){
            case 0:
                if(args_array[0] != NULL){
                    printf("Error at line %d: too many arguments\n",line_counter);
                    free(tmp_instruction);
                    free(error);
                    free(args_array[0]);
                    free(args_array[1]);
                    return FALSE;                
                }
                tmp_instruction->ARE = 0;
                tmp_instruction->dest_add = 0;
                tmp_instruction->op_code = ins_code;
                tmp_instruction->source_add = 0;

                rapping_word = (instruction_word *)tmp_instruction;

                if(!(add_to_inst_list(instruction_image,rapping_word))){
                    printf("Error at line %d: unable to allocate memory\n",line_counter);
                    free(tmp_instruction);                    
                    free(error);
                    free(args_array[0]);
                    free(args_array[1]);
                    return FALSE;
                }
                (*instruction_counter)++;
                free(error);
                free(args_array[0]);
                free(args_array[1]);
                return result;

            case 1:
                if(args_array[1] != NULL){
                    printf("Error at line %d: too many arguments\n",line_counter);
                    free(tmp_instruction);
                    free(error);
                    free(args_array[0]);
                    free(args_array[1]);
                    return FALSE;                
                }
                
                f_rand_add = parse_single_oprand(args_array[0],error,first_rand);
                if((f_rand_add < 1) || !(valid_addressing(f_rand_add,*ligal_add_source))){
                    printf("Error at line %d: %s\n",line_counter,error);
                    free(tmp_instruction);
                    free(first_rand);
                    free(error);
                    free(args_array[0]);
                    free(args_array[1]);         
                    return FALSE;
                }
                /*If ligal instruction and after that the oprand*/
                tmp_instruction->ARE = 0;
                tmp_instruction->dest_add = (unsigned int)(f_rand_add);
                tmp_instruction->op_code = ins_code;
                tmp_instruction->source_add = 0;
                rapping_word = (instruction_word *)tmp_instruction;
                
                if(!(add_to_inst_list(instruction_image,rapping_word))){
                    printf("Error at line %d: unable to allocate memory\n",line_counter);
                    free(tmp_instruction);
                    free(first_rand);
                    free(error);
                    free(args_array[0]);
                    free(args_array[1]);  
                    return FALSE;
                }
                (*instruction_counter)++;

                if(!(add_to_inst_list(instruction_image,first_rand))){
                    printf("Error at line %d: unable to add %s to instruction image\n",line_counter,args_array[0]);
                    remove_last_inst(instruction_image);
                    free(tmp_instruction);
                    free(first_rand);
                    free(error);
                    free(args_array[0]);
                    free(args_array[1]);
                    (*instruction_counter)--;
                    return FALSE;
                }
                (*instruction_counter)++;
                return result;

            case 2:
                if(args_array[1] == NULL){
                    printf("Error at line %d: not enough arguments\n",line_counter);
                    free(tmp_instruction);
                    free(first_rand);
                    free(error);
                    free(args_array[0]);
                    free(args_array[1]);  
                    return FALSE;   
                }

                
                f_rand_add = parse_single_oprand(args_array[0],error,first_rand);
                if(f_rand_add < 1 || !(valid_addressing(f_rand_add,*ligal_add_source))){
                    printf("Error at line %d: %s\n",line_counter,error);
                    free(tmp_instruction);
                    free(first_rand);
                    free(error);
                    free(args_array[0]);
                    free(args_array[1]);                 
                    return FALSE;
                }

                
                s_rand_add = parse_single_oprand(args_array[1],error,second_rand);
                if(s_rand_add < 1 || !(valid_addressing(s_rand_add,*ligal_add_dest))){
                    printf("Error at line %d: %s\n",line_counter,error);
                    free(tmp_instruction);
                    free(first_rand);
                    free(second_rand);
                    free(error);
                    free(args_array[0]);
                    free(args_array[1]);              
                    return FALSE;
                }

                /*If ligal instruction and after that the oprand*/
                
                tmp_instruction->ARE = 0;
                tmp_instruction->dest_add = (unsigned int)(s_rand_add);
                tmp_instruction->op_code = ins_code;
                tmp_instruction->source_add = (unsigned int)(f_rand_add);

                rapping_word = (instruction_word *)tmp_instruction;
                
                if(!(add_to_inst_list(instruction_image,rapping_word))){
                    printf("Error at line %d: unable to allocate memory\n",line_counter);
                    free(tmp_instruction);
                    free(first_rand);
                    free(second_rand);
                    free(error);
                    free(args_array[0]);
                    free(args_array[1]);
                    return FALSE;
                }
                (*instruction_counter)++;

                if(f_rand_add == REGISTER && s_rand_add == REGISTER){/*Both are registers we can use the same line*/
                    combine_register(first_rand,second_rand);
                    free(first_rand);

                    if(!(add_to_inst_list(instruction_image,first_rand))){
                        printf("Error at line %d: unable to add %s to instruction image\n",line_counter,args_array[0]);
                        remove_last_inst(instruction_image);
                        free(tmp_instruction);
                        free(first_rand);
                        free(second_rand);
                        free(error);
                        free(args_array[0]);
                        free(args_array[1]);
                        (*instruction_counter)--;
                        result = FALSE;
                    }
                    (*instruction_counter)++;
                    return result;           
                }


                if(!(add_to_inst_list(instruction_image,first_rand))){
                    printf("Error at line %d: unable to add %s to instruction image\n",line_counter,args_array[0]);
                    remove_last_inst(instruction_image);
                    (*instruction_counter)--;
                    free(tmp_instruction);
                    free(first_rand);
                    free(second_rand);
                    free(error);
                    free(args_array[0]);
                    free(args_array[1]);
                    return FALSE;
                }
                (*instruction_counter)++;
                
                if(!(add_to_inst_list(instruction_image,second_rand))){
                    printf("Error at line %d: unable to add %s to instruction image\n",line_counter,args_array[1]);
                    remove_last_inst(instruction_image);
                    remove_last_inst(instruction_image);
                    (*instruction_counter)-= 2;
                    free(tmp_instruction);
                    free(first_rand);
                    free(second_rand);
                    free(error);
                    free(args_array[0]);
                    free(args_array[1]);
                    return FALSE;
                }
                (*instruction_counter)++;
            }
        }
    free(error);
    free(args_array[0]);
    free(args_array[1]);
    return result;
}

bool add_to_data_list(data_list *list, data_word *data){
    data_node *new_node;
    new_node = (data_node *)malloc(sizeof(data_node));
    if (new_node == NULL){
        printf("Error: unable to allocate memory\n");
        return FALSE;
    }
    new_node->data = data;
    new_node->next = NULL;
    if (list->head == NULL){
        list->head = new_node;
        list->tail = new_node;
        return TRUE;
    }
    else{
        list->tail->next = new_node;
        list->tail = new_node;
        return TRUE;
    }
}

bool add_to_inst_list(inst_list *list, instruction_word *inst){
    inst_node *new_node = (inst_node *)malloc(sizeof(inst_node));
    if (new_node == NULL){
        printf("Error: unable to allocate memory\n");
        return FALSE;
    }
    new_node->inst = inst;
    new_node->next = NULL;
    if (list->head == NULL){
        list->head = new_node;
        list->tail = new_node;
        return TRUE;
    }
    else{
        list->tail->next = new_node;
        list->tail = new_node;
        return TRUE;
    }

}

inst_list *init_inst_list(){
    inst_list *new_list = (inst_list *)malloc(sizeof(inst_list));
    if (new_list == NULL){
        printf("Error: unable to allocate memory\n");
        return NULL;
    }
    new_list->head = NULL;
    new_list->tail = NULL;
    return new_list;
}

data_list *init_data_list(){
    data_list *new_list = (data_list *)malloc(sizeof(data_list));
    if (new_list == NULL){
        printf("Error: unable to allocate memory\n");
        return NULL;
    }
    new_list->head = NULL;
    new_list->tail = NULL;
    return new_list;
}

void free_data_list(data_list *list){
    data_node *tmp = list->head;
    while(tmp != NULL){
        list->head = list->head->next;
        free(tmp->data);
        free(tmp);
        tmp = list->head;
    }
    free(list);
}

void free_inst_list(inst_list *list){
    inst_node *tmp = list->head;
    while(tmp != NULL){
        list->head = list->head->next;
        free(tmp->inst);
        free(tmp);
        tmp = list->head;
    }
    free(list);
}

void remove_last_inst(inst_list *instruction_image){
    inst_node *tmp = instruction_image->head;
    inst_node *prev = NULL;

    while (tmp->next != NULL)
    {
        prev = tmp;
        tmp = tmp->next;
    }
    free(tmp->inst);
    free(tmp);
    prev->next = NULL;
    instruction_image->tail = prev;
}

unsigned int extract_bits(void *word_ptr){
    word *tmp_word = NULL;
    unsigned int result;
    tmp_word = (word *)word_ptr;
    result = *(unsigned int *)tmp_word;
    return result; 
}

unsigned int get_inst_val(inst_node *inst){
    return extract_bits(inst->inst);
}
unsigned int get_data_val(data_node *data){
    return extract_bits(data->data);
}

inst_node *get_next_inst(inst_node *inst){
    return inst->next;
}
data_node *get_next_data(data_node *data){
    return data->next;
}

inst_node *get_head_inst(inst_list *list){
    return list->head;
}
data_node *get_head_data(data_list *list){
    return list->head;
}
