/*
This file is the implementation of the ast.h file
It contains the implementation of all the data structures used in the program
Instruction struct
Gideness struct
Macro struct
*/
#include "ast.h"

bool get_args(char *args, list *arg_array, int *line_counter){
     
    bool reading_arg = FALSE;
    bool no_delimiter = FALSE;
    bool last_comma = FALSE;
    node *tmp_node = NULL;
    char *buffer[MAX_LABEL_LENGTH+1];/*Too long arguments will be checked by context*/
    bool result = TRUE;
    strcpy(buffer,"");

    while(args != '\n'){
        if(args == ','){
            if(reading_arg){
                reading_arg = FALSE;/*End of reading arg*/
                no_delimiter = FALSE;
                tmp_node = node_init(buffer);
                if(tmp_node == NULL){
                    printf("Error at line %d: unable to allocate memory\n",*line_counter);
                    result = FALSE;
                }
                add_to_list(arg_array,tmp_node);
                strcpy(buffer,"");
                tmp_node = NULL;
            }
            else
            {
                printf("Error at line %d: comma must come after an argument\n",*line_counter);
                result = FALSE;
            }
            last_comma = TRUE;
        }

        else if (isspace((unsigned char)*args)){/*Handle white spaces*/
            if(reading_arg)
            {
                if(strcmp(buffer,"-") == 0 || strcmp(buffer,"+") == 0){/*if sign is not followed by a number*/
                    printf("Error at line %d: %s must be followed by a number\n",*line_counter,buffer);                   
                }                
                no_delimiter = TRUE;
            }
        }/*End of white space*/

        else{/*We have a char*/                    
            if(no_delimiter){
                printf("Error at line %d: %s must be followed by a delimiter\n",*line_counter,buffer);
                result = FALSE;
                tmp_node = node_init(buffer);
                if(tmp_node == NULL){
                    printf("Error at line %d: unable to allocate memory\n",*line_counter);
                    result = FALSE;
                }
                add_to_list(arg_array,tmp_node);
                strcpy(buffer,"");
                tmp_node = NULL;
                strcat(buffer,args);
            }

            else{
                last_comma = FALSE;
                reading_arg = TRUE;
                strcpy(buffer,args);
            } 
        }
        args++;        
    }

    if (reading_arg){
        tmp_node = node_init(buffer);
        if(tmp_node == NULL){
            printf("Error at line %d: unable to allocate memory\n",*line_counter);
            result = FALSE;
        }
        add_to_list(arg_array,tmp_node);
        strcpy(buffer,"");
        tmp_node = NULL;
    }
    else (last_comma){
        printf("Error at line %d: line can't end with a comma\n",*line_counter);
        result = FALSE;
    }
    return result;
}


bool is_label(char *first_frase){
    char lable_char = ':';
    /*Checks the word end with ':'*/
    int lable_len = strlen(first_frase);
    if (first_frase[lable_len-1] != lable_char){
        return FALSE;
    }    
    return TRUE;
}

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


bool init_data_in_data(data_list *data_image,long *data_counter,int *num,int *line_counter){
    bool error = TRUE;
    int defualt_val = 0;
    if(!legal_data_num(num)){
        printf("Error at line %d: %d is not a legal number\n",*line_counter,num);
        num = defualt_val;
    }
    data_word *d_word =  (data_word *)malloc(sizeof(data_word));
    if (d_word == NULL){
        printf("Error: unable to allocate memory\n");
        return FALSE;
    }
    data_node *result = (data_node *)malloc(sizeof(data_node));
    if (result == NULL){
        printf("Error: unable to allocate memory\n");
        free(d_word);
        return FALSE;
    }
    d_word->data = num;
    result->data = d_word;
    error = add_to_data_list(data_image,result);
    (*data_counter)++;
    return error;

}

bool parse_data_guid(list *args,data_list *data_image,long *data_counter,int *line_counter)/*TODO go over this function*/
{
    bool result = TRUE;
    data_node *tmp_node = NULL;
    int i = 0;
    int *num = 0;
    char *buffer[MAX_LINE_LENGTH]="";/*buffer for the number*/       
    bool dubble_comma = TRUE;/*first arg can't be a comma*/
    bool no_delimiter = FALSE; /*Flag that locates 2 diffrent numbers without a delimiter*/ 
    bool reading_arg = FALSE;
    node *tmp_node = get_list_head(args);
    if(tmp_node == NULL){
        printf("Error at line %d: .data command must have at least one argument\n",*line_counter);
        return FALSE;
    }

    while(tmp_node != NULL){
        strcpy(buffer,get_data(tmp_node));                                     
        (*num) = atoi(buffer);
        if(!(init_data_in_data(data_image,data_counter,num,line_counter))){
            result = FALSE;
        }
        tmp_node = get_next(tmp_node);         
    }

    return result;
}

bool parse_string_guid(char *args,data_list *data_image,long *data_counter,int *line_counter){/*TODO go over this function*/
    bool insert = FALSE;
    int quote_counter = 0;
    int i = 0;
    int *val = 0;
    while(args != '/n'){
        if(args == '"'){
            if(quote_counter > 1){
                printf("Error at line %d: string can't have more than 1 argument %s will not be added to data image\n",*line_counter,args);
            }
            else if (insert){/*closing quote*/
                insert = FALSE;
                val = 0;
                if(!(init_data_in_data(data_image,data_counter,val,line_counter))){
                    return FALSE;
                }
                (*data_counter)++;
                quote_counter++;
            } 
            else{/*opening quote*/
                quote_counter++;
                insert = TRUE;
            }
        }        
        else if(insert){
            val = (int)args;
            if(!(init_data_in_data(data_image,data_counter,val,line_counter))){
                return FALSE;
            }
            (*data_counter)++;
        } 
        else if(!(isspace((unsigned char)*args))){            
            if(quote_counter){
                printf("Error at line %d: only one argument is allowed in a .string %s will not be added to data image\n",*line_counter,args); 
                return FALSE;
            }
            else{
                printf("Error at line %d: .string command must start with a \" %s will not be added to data image\n",*line_counter,args);
                return FALSE;
            }
        }
        args++;            
    }
}

bool parse_extern(char *args, symbol_list *symbol_table,int *line_counter){/*TODO go over this function*/
    bool result = TRUE;
    char *error = "";
    char *buuffer[MAX_LABEL_LENGTH+1] = args;
    if (args == NULL){
        printf("Error at line %d: .extern command must have at least one argument\n",*line_counter);
        return FALSE;
    }
    bool reading = FALSE;
    bool last_comma = TRUE; 

    while (args != '\n'){
        if(args == ','){
            if(reading){                
                add_symbol(symbol_table,buuffer,-1);
                if(strcmp(error,"") == 0){
                    result = FALSE;
                    printf("Error at line %d: %s",*line_counter,error);
                }
            }
            else{
                result = FALSE;
                printf("Error at line %d: .extern command must start with a label\n",*line_counter);
            }            
            reading = FALSE;
            last_comma = TRUE;
        }

        else if(isspace((unsigned char)*args)){
            if(reading){
                if(last_comma){
                    result = FALSE;
                    printf("Error at line %d: .extern command must start with a label\n",*line_counter);
                }
                else{
                    add_symbol(symbol_table,buuffer,-1);
                    if(strcmp(error,"") == 0){
                        result = FALSE;
                        printf("Error at line %d: %s",*line_counter,error);
                    }
                }
            }
        }

        else if(isalpha((unsigned char)*args)){
            if(reading){
                strcat(buuffer,args);
            }
            else{
                reading = TRUE;
                strcpy(buuffer,args);
            }
        }
    return result;
}

int find_op_code(char *op_code){
    int result = 1;/*Helping determine if the instruction is legal / ligal but not all lower case */
    char *error;
    switch (strcmp(lower_op_code,case)){
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
    return -1;
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

word *parse_single_oprand(char *args,char *error_msg){
    int result = 0;
    char *error_format ="";
    word *tmp_word = malloc(sizeof(word));
    if(tmp_word == NULL){
        strcat(error_msg,"Error: unable to allocate memory\n");
        exit(1);
    }
    if (args == NULL){
        strcat(error_msg,"Error: missing argument\n");
        free(tmp_word);
        return -1;
    }
    
    if(args[0]== '@'){/*Arg is a sespected */
        if(args[1] == 'r' && args[2] >= '0' && args[2] <= '7' && args[3] == '\0'){
            register_word *rand_word = malloc(sizeof(register_word));
            if(rand_word == NULL){
                strcat(error_msg,"Error: unable to allocate memory\n");
                exit(1);
            }
            rand_word->ARE = 0;
            rand_word->source_reg = 0;
            rand_word->dest_reg = args[2] - '0';

            tmp_word->label = NULL;
            tmp_word->word_type = REGISTER;
            tmp_word->word = rand_word;            
            tmp_word->error = error_msg;
            return tmp_word;
            }

        else{/*Invalid register won't be added to image*/
            sprintf(error_format,"Error: %s is invalid register\n",args);
            strcat(error_msg,error_format);
            free(tmp_word);
            return NULL;
        }
    }
    else if(isalpha(args[0])){/*Arg is a label*/
            immediate_direct_word *rand_word = malloc(sizeof(immediate_word));
            if(rand_word == NULL){
                strcat(error_msg,"Error: unable to allocate memory\n");
                free(tmp_word);
                return NULL;
            }

            rand_word->ARE = 0;
            rand_word->dest_add = 0;

            tmp_word->label = args;
            word->word_type = DIRECT;            
            tmp_word->word = rand_word;
            tmp_word->error = error_msg;
            return tmp_word;
        }
        
    else{
        int *num = atoi(args);
        if(*num < MIN_IMMEDIATE_VALUE || *num > MAX_IMMEDIATE_VALUE){
            sprintf(error_format,"Error: %s is invalid immediate value\n",args);
            strcat(error_msg,error_format);
            free(tmp_word);
            return NULL;
        }
        else{
            immediate_word *rand_word = malloc(sizeof(immediate_word));
            if(rand_word == NULL){
                strcat(error_msg,"Error: unable to allocate memory\n");
                free(tmp_word);
                return null;
            }
            if (num < 0 )
            {
                *num = complement_2(num);
            }

            rand_word->ARE = 0;
            rand_word->dest_add = *num;

            tmp_word->label = NULL;
            tmp_word->word_type = IMMEDIATE;
            tmp_word->word = rand_word;
            tmp_word->error = error_msg;
            return tmp_word;            
        }
    }
}

void set_ligal_params(int *ins_code, int *ligal_add_source, int *ligal_add_dest, int *word_limit){    
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

bool valid_addressing(int *given_addressing, int *ligal_addressing)
{
    switch (*ligal_addressing)
    {/*Since the ligal addresings are 1,3,5 || 3,5 || 3, we and the sum of each is unique, we can didacte the ligal addressing by the sum of the ligal addressing*/
    case 9:
        if(*given_addressing == 1 || *given_addressing == 3 || *given_addressing == 5)
            return TRUE;
        else
            return FALSE;
        break;

    case 8:
        if(*given_addressing == 3 || *given_addressing == 5)
            return TRUE;
        else
            return FALSE;
        break;

    case 3:
        if(*given_addressing == 3)
            return TRUE;
        else
            return FALSE;
        break;
    
    default:/*For debuging*/
        printf("Something is wrong at valid_addressing function given %d and %d\n",*given_addressing,*ligal_addressing);
        retun FALSE;
        break;
    }
}

bool parse_instruction(int *op_code, char **args[], instruction_word **instruction_image[],long *instruction_counter,int *line_counter){
    word *rapping_word = malloc(sizeof(word));
    if(rapping_word == NULL){
        strcat(error_msg,"Error: unable to allocate memory\n");
        exit(1);
    }
    instruction_op_word *tmp_instruction = malloc(sizeof(instruction_op_word));
    if(tmp_instruction == NULL){
        strcat(error_msg,"Error: unable to allocate memory\n");
        exit(1);
    }
    bool result = (strcmp(error_msg,"") == 0)? TRUE:FALSE;
    int *word_limit = 0;
    char *error;
    int *ligal_add_source = 0;
    int *ligal_add_dest = 0;
    char *args_array[3]= {NULL,NULL,NULL};
    get_args(args,args_array);
    set_ligal_params(ins_code,ligal_add_source,ligal_add_dest,word_limit);

    switch(*word_limit){
        case 0:
            if(strcmp((args_array[0],"") != 0) || (strcmp((args_array[2],"") != 0))){
                strcat(error_msg,args_array[2]);
                strcat(error_msg,"Error: too many arguments\n");
            }
            tmp_instruction->ARE = 0;
            tmp_instruction->dest_add = 0;
            tmp_instruction->op_code = ins_code;
            tmp_instruction->source_add = 0;

            rapping_word->label = NULL;
            rap_word->word_type = 0;
            rapping_word->word = tmp_instruction;
            rapping_word->error = error_msg;
            instruction_image[*instruction_counter] = rapping_word;
            *instruction_counter++;

            if(strcmp((error_msg,"") != 0)){
                result = FALSE;
            }
            return result;

        case 1:
            if(strcmp((args_array[2],"") != 0 || (strcmp((args_array[1],"") != 0)))){
                strcat(error_msg,args_array[2]);
                strcat(error_msg,"Error: too many arguments\n");
                result = FALSE;
            }/*TODO: decide when to stop else*/

            word *first_rand = parse_single_oprand(args_array[0],"");
            if(!(valid_addressing(first_rand->word_type,ligal_add_dest))){
                strcat(error_msg,sprintf("Error: the %s is not a ligal addressing in this instruction\n",args_array[0]));
                free(first_rand->word);
                free(first_rand);
            }  
            tmp_instruction->ARE = 0;
            tmp_instruction->dest_add = *first_rand->word_type;
            tmp_instruction->op_code = ins_code;
            tmp_instruction->source_add = 0;

            rapping_word->label = NULL;
            rap_word->word_type = 0;
            rapping_word->word = tmp_instruction;
            rapping_word->error = error_msg;
            instruction_image[*instruction_counter] = rapping_word;
            *instruction_counter++;        

            if(first_rand->error != NULL){/*We will add the rand to the instruction but show the error*/
                strcat(error_msg,first_rand->error);
                strcat(rapping_word->error,first_rand->error);
            }

            instruction_image[*instruction_counter] = first_rand;
            *instruction_counter++;    
        
            if(strcmp((error_msg,"") != 0)){
                result = FALSE;
            }
        

            return result;

        case 2:
            if(strcmp((args_array[2],"") != 0)){
                strcat(error_msg,args_array[2]);
            }
            else if(strcmp((args_array[1],"") == 0)){
                strcat(error_msg,"Error: missing arguments\n");
            }
            
            word *first_rand = parse_single_oprand(args_array[0],"");
            if(!(valid_addressing(first_rand->word_type,ligal_add_source))){
                strcat(error_msg,sprintf("Error: the %s is not a ligal addressing in this instruction\n",args_array[0]));
                free(first_rand->word);
                free(first_rand);
            }       

            word *second_rand = parse_single_oprand(args_array[1],"");
            if(!(valid_addressing(second_rand->word_type,ligal_add_dest))){
                strcat(error_msg,sprintf("Error: the %s is not a ligal addressing in this instruction\n",args_array[1]));
                free(second_rand->word);
                free(second_rand);
            }

                    
            tmp_instruction->ARE = 0;
            tmp_instruction->dest_add = *first_rand->word_type;
            tmp_instruction->op_code = ins_code;
            tmp_instruction->source_add = *second_rand->word_type;


            rapping_word->label = NULL;
            rap_word->word_type = 0;
            rapping_word->word = tmp_instruction;
            rapping_word->error = error_msg;
            instruction_image[*instruction_counter] = rapping_word;
            *instruction_counter++;        

            if(first_rand->error != NULL || second_rand->error != NULL ){/*We will add the rand to the instruction but show the error*/
                strcat(error_msg,first_rand->error);
                strcat(rapping_word->error,first_rand->error);
            }
            if(first_rand->word_type == REGISTER){/*first is register we should change it to be the source and make the dest the zero register*/
                first_rand->word->source_add = first_rand->word->dest_add;
                first_rand->word->dest_add = 0;
                if(second_rand->word_type == REGISTER){/*Both are registers we can use the same line*/
                    first_rand->word->dest_add = second_rand->word->dest_add;
                    free(second_rand->word);
                    free(second_rand);
                }
            }

            if(second_rand == NULL){
                instruction_image[*instruction_counter] = first_rand;
                *instruction_counter++;    
            }
            else{
                instruction_image[*instruction_counter] = first_rand;
                *instruction_counter++;    
                instruction_image[*instruction_counter] = second_rand;
                *instruction_counter++;    
            }  
        
            if(strcmp((error_msg,"") != 0)){
                result = FALSE;
            }
            return result;

        default:
            strcat(error_msg,"Something went wrong with function parse_instruction");
            return FALSE;
    }
}

bool add_to_data_list(data_list *list, data_word *data){
    if (list->head == NULL){
        list->head = data;
        list->tail = data;
        return TRUE;
    }
    else{
        list->tail->next = data;
        list->tail = data;
        return TRUE;
    }
    return FALSE;
}

bool add_to_inst_list(inst_list *list, instruction_word *inst){
    if (list->head == NULL){
        list->head = inst;
        list->tail = inst;
        return TRUE;
    }
    else{
        list->tail->next = inst;
        list->tail = inst;
        return TRUE;
    }
    return FALSE;
}


void free_data_list(data_list *list){
    data_word *tmp = list->head;
    while(tmp != NULL){
        list->head = list->head->next;
        free(tmp->data);
        free(tmp);
        tmp = list->head;
    }
}

void free_inst_list(inst_list *list){
    instruction_word *tmp = list->head;
    while(tmp != NULL){
        list->head = list->head->next;
        free(tmp->inst);
        free(tmp);
        tmp = list->head;
    }
}