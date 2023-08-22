/*
This file diclares the functions that will be used to generate the files in the case of a valid assembly code.
*/
#ifndef FILE_GENERATOR_H
#define FILE_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "globals.h"
#include "table_generator.h"
#include "ast.h"

/* This function generates the .ob file 
@ param file_name - the name of the file to be generated
@ param instruction_counter - the number of instructions in the code
@ param data_counter - the number of data words in the code
@ param code_image - the array of words that represent the instructions
@ param data_image - the array of data words that represent the data
*/

void generate_ob_file(char *file_name, long instruction_counter, long data_counter, instruction_word code_image[], data_word data_image[]);




/* This function generates the .ent file a file that contains the names of the labels that are used as entry points
@ param file_name - the name of the file to be generated
@ param symbol_table - the symbol table
@ return void

*/

void generate_ent_file(char *file_name, symbol_list *symbol_list);


/* This function generates the .ext file a file that contains the names of the labels that are used as external labels
@ param file_name - the name of the file to be generated
@ param symbol_table - the symbol table
@ param code_image - the array of words that represent the instructions
@ param instruction_counter - the number of instructions in the code
*/
void generate_ext_file(char *file_name, symbol_list *symbol_list, instruction_word code_image[], long instruction_counter);

/* This function generates the files in the case of a valid assembly code.*/

void generate_all_files(char *file_name, symbol_list *symbol_table,instruction_word  code_image[], data_word data_image[], long instruction_counter, long data_counter);

#endif
