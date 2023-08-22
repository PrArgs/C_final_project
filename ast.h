/*AST abstruct syntacs tree get a line from the parser and brakes it to the basic componantes.
*/

/*This function will get a line from the parser and brakes it to the basic componantes.*/
#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "table_generator.h"
#include "util.h"
#include "machine_code.h"
#include "data_structures.h"

#define MAX_DATA_VALUE 2047
#define MIN_DATA_VALUE -2048
#define MAX_IMMEDIATE_VALUE 511
#define MIN_IMMEDIATE_VALUE -512

/*Deffing the types of the commands*/
typedef enum op_codes{
    MOV=0,
    CMP=1,
    ADD=2,
    SUB=3,
    NOT=4,
    CLR=5,
    LEA=6,
    INC=7,
    DEC=8,
    JMP=9,
    BNE=10,
    RED=11,
    PRN=12,
    JSR=13,
    RTS=14,
    STOP=15
    } op_code;

typedef enum guid_op{
    data=0,
    string=1,
    entry=2,
    extern_guid_op=3
}guid_op;

/*Deffing the types of the registers*/
typedef enum registers{
    R0=0,
    R1=1,
    R2=2,
    R3=3,
    R4=4,
    R5=5,
    R6=6,
    R7=7,
    NOT_REG = -1
    } _register;

/*Deffing the types of the addressing methods*/
typedef enum addressing_methods{
    IMMEDIATE=1,
    DIRECT=3,
    REGISTER = 5
    } addressing_method;

/*Deffing the incoding methods)*/
typedef enum incoding_methods{
    A=0,
    E=1,
    R=2
    } incoding_method;


/*Define data word*/
typedef struct data_word{
    unsigned int data:12;
    } data_word;

/*Define register word*/
typedef struct register_word{
    unsigned int ARE:2;
    unsigned int source_reg:5;
    unsigned int dest_reg:5;
    } register_word;

/*Define immediate direct word*/
typedef struct immediate_direct_word{
    unsigned int ARE:2;
    unsigned int operand:10;
    } immediate_direct_word;

/*Define guidance word*/

/*Define data word a word generated by an guidance command as 
as apoosed to a word generated by an instruction command*/
typedef struct guidance_word{
    char *guidance_op;
    char *args;
    } guidance_word;

/*Deffing an instruction word*/
typedef struct instruction_op_word{
    unsigned int ARE:2;
    unsigned int dest_add:3;
    unsigned int op_code:4;
    unsigned int source_add:3;
    } instruction_op_word;


typedef union {
    instruction_op_word instruction_op_word;
    immediate_direct_word immediate_direct_word;
    register_word register_word;
} inst_word;

/*Deffing an instruction word*/
typedef struct instruction_word {
     inst_word *i_word;
} instruction_word;

typedef struct word{
    char *label;
    int *word_type;
    union *w_word{
        data_word *data_word;
        register_word *register_word;
        immediate_direct_word *immediate_direct_word;
        instruction_word *instruction_word;
        instruction_op_word *instruction_op_word;
        guidance_word *guidance_word;
        } w_word;
    char *error;
    } word;

tpdedef struct data_node{
    data_word *data;
    struct data_node *next;
}data_node;

typedef struct data_list{
    data_node *head;
    data_node *tail;
}data_list;

bool add_to_data_list(data_list *list, data_word *data);

typedef struct inst_node{
    instruction_word *inst;
    struct inst_node *next;
}inst_node;

typedef struct inst_list{
    inst_node *head;
    inst_node *tail;
}inst_list;

bool add_to_inst_list(inst_list *list, instruction_word *inst);

void free_data_list(data_list *list);
void free_inst_list(inst_list *list);




/*Deffing a word*/
typedef struct ast_line{
    char *label;
    word *word[3];
    char *error;
    } ast_line;


/*
################### THE CONCRETE SYNTAX #######################
ast-exp:= {lable* , line-exp, error-exp}
line-exp:= {instruction-exp | guidance-exp}
instruction-exp:= {op-code-exp+, ARE-exp, reemaing-words}
guidance-exp:= {guidance-op-exp ,data-exp+ ,ARE-exp}
op-code-exp:= {op_code , rand-exp*}
rand-exp:= {{register | immediate-direct-exp | data-word-exp},{ARE-exp}}
val-exp:= {add-exp | oprnd-exp}
add-exp:= {register-exp | immediate-direct-exp | data-word-exp}
oprnd-exp:= {register | symbol-exp | immediate-val-exp}
register:= {R0 | R1 | R2 | R3 | R4 | R5 | R6 | R7 | NOT_REG}
symbol-exp:= {label, value}
value:= {number}
immediate-val-exp:= {data-word-exp}
remaining-words:= {char*}
guidance-op-exp:= {.string | .data | .entry | .extern}
ARE-exp:= {A | R | E}
*/


typedef struct line{
    union line_type{
        instruction_word *instruction_exp;
        guidance_word *guidance_line;
    } line_type;
    char *error;
} line;

/*
ast_line *line_to_ast(char *line, long line_number);

list *line_to_list(char *line, long line_number);

ast_line *ast_to_word(char *line);
*/

/*This part will contain function designed for checking legality and set error if needed*/
bool is_label(char *first_frase);
bool ligal_label(char *first_frase);
ast_line *build_ast(char *current_line,symbol_list *symbol_table,long *data_counter,long *instruction_counter,char *error_msg,char *tmp_lable ,data_word *data_image[], instruction_word *instruction_image[]);
bool is_guidance_of_label(char *frase);




void set_ligal_params(int *ins_code, int *ligal_add_source, int *ligal_add_dest, int *word_limit);

word *parse_single_oprand(char *args,char *error_msg);

void get_args(char *args,char *array[]);

bool *valid_addressing(int *given_addressing, int *ligal_addressing);

bool parse_data_guid(char *args,data_word *data_image[],long *data_counter,int *line_counter);

bool parse_string_guid(char *args,data_word *data_image[],long *data_counter,int *line_counter);

bool parse_extern(char *args, symbol_list *symbol_table,int *line_counter);

bool parse_instruction(int *op_code, char **args[], instruction_word *instruction_image[],long *instruction_counter,int *line_counter);

void free_ins_array(instruction_word instruction_image[], long *ic);

void free_data_array(data_word *data_image[], long *dc);

bool add_data(data_word **data_image[], long *data_counter, data_word *data);

bool add_instruction(instruction_word **instruction_image[], long *instruction_counter, instruction_word *instruction);


#endif