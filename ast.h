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



typedef struct data_node data_node;
typedef struct data_list data_list;
typedef struct inst_node inst_node;
typedef struct inst_list inst_list;


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