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
#include "data_structures.h"

/*If an identifier has no linkage, there shall be no more than one declaration of the identifier (in a declarator or type specifier) 
with the same scope and in the same name space, except for tags as specified in 6.7.2.3.

hence i have to declare the structers here and not in the ast.c file which normaly will be better in terms of encapsulation and seperation of concerns.
*/




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


typedef struct instruction_op_word{
    unsigned int ARE:2;
    unsigned int dest_add:3;
    unsigned int op_code:4;
    unsigned int source_add:3;
    } instruction_op_word;

typedef union m_word {
    register_word reg_word;
    immediate_direct_word imm_direct_word;
    instruction_op_word inst_op_word;
    data_word data_word;
} m_word;

typedef struct word {
    m_word m_word;
} word;


/* Define union for instruction word types */
typedef union inst_word {
    register_word reg_word;
    immediate_direct_word imm_direct_word;
    instruction_op_word inst_op_word;
} inst_word;

/* Define struct for instruction words */
typedef struct instruction_word {
    inst_word word_type;
} instruction_word;

typedef struct data_node{
    data_word *data;
    struct data_node *next;
} data_node;

typedef struct data_list{
    data_node *head;
    data_node *tail;
} data_list;

typedef struct inst_node{
    instruction_word *inst;
    struct inst_node *next;
} inst_node;

typedef struct inst_list{
    inst_node *head;
    inst_node *tail;
} inst_list;


bool parse_data_guid(list *args, data_list *data_image, long *data_counter, int line_counter);
bool get_args(char *args, list *arg_array, int line_counter);
bool is_label(char *first_frase);
bool parse_string_guid(list *args, data_list *data_image, long *data_counter, int line_counter);
bool parse_extern(list *args, symbol_list *symbol_table, int line_counter,char *error_msg);
inst_list *init_inst_list();
data_list *init_data_list();
bool parse_instruction(int ins_code, list *args, inst_list *instruction_image, long *instruction_counter, int line_counter);
void free_data_list(data_list *list);
void free_inst_list(inst_list *list);
unsigned int extract_bits(void *word_ptr);
int find_op_code(char *op_code);
inst_node *get_next_inst(inst_node *inst);
inst_node *get_i_inst(inst_list *list, long i);
bool set_i_inst(inst_list *list, long i,char *symbol_name, symbol_list *symbol_table);
data_node *get_next_data(data_node *data);
inst_node *get_head_inst(inst_list *list);
data_node *get_head_data(data_list *list);
unsigned int get_inst_val(inst_node *inst);
unsigned int get_data_val(data_node *data);


bool is_lable_rand(char *arg);
int lable_op_code(int op_code);



#endif