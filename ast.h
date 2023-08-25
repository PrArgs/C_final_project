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






bool ligal_label(char *first_frase);
bool legal_indirect_num(int num);
bool legal_reg(char *reg);
bool legal_data_num(int num);
bool legal_char(char c);
bool init_data_in_data(data_list *data_image, long *data_counter, int num, int line_counter);
void set_ligal_params(int ins_code, int *ligal_add_source, int *ligal_add_dest, int *word_limit);
bool valid_addressing(int given_addressing, int ligal_addressing);

bool get_args(char *args, list *arg_array, int line_counter);
bool is_label(char *first_frase);
bool parse_data_guid(list *args, data_list *data_image, long *data_counter, int line_counter);
bool parse_string_guid(list *args, data_list *data_image, long *data_counter, int line_counter);
bool parse_extern(list *args, symbol_list *symbol_table, int line_counter,char *error_msg);
int find_op_code(char *op_code);
bool parse_entry(list *args, symbol_list *symbol_table, char *error_msg, int line_counter);
bool parse_instruction(int ins_code, list *args, inst_list *instruction_image, long *instruction_counter, int line_counter);
bool add_to_data_list(data_list *list, data_word *data);

bool valid_addressing(int given_addressing, int ligal_addressing);
unsigned int get_inst_val(inst_node *inst);
unsigned int get_data_val(data_node *data);
inst_list *init_inst_list();
data_list *init_data_list();
inst_node *get_next_inst(inst_node *inst);
inst_node *get_i_inst(inst_list *list, long i);
bool set_i_inst(inst_list *list, long i,char *symbol_name, symbol_list *symbol_table);
data_node *get_next_data(data_node *data);
inst_node *get_head_inst(inst_list *list);
data_node *get_head_data(data_list *list);
void free_data_list(data_list *list);
void free_inst_list(inst_list *list);
void remove_last_inst(inst_list *instruction_image);
unsigned int extract_bits(void *word_ptr);

bool is_lable_rand(char *arg);
int lable_op_code(int op_code);



#endif