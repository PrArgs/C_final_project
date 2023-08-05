/*This file contains the fuctions that will be used to convert the assembly code into machine code.
one for instructions.
and one for each type of directives.
*/

#include <stdlib.h>
#include "globals.h"
#include "table_generator.h"
#include "ast.h"


/*This function incode the instruction into binary code.
@param instruction - the line of the instruction
@param symbol_table - the symbol table
@param code_image - the array of words that represent the instructions
@param instruction_counter - the number of instructions in the code
@param data_image - the array of data words that represent the data
@param data_counter - the number of data words in the code
@param AST - the abstract syntacs tree of the instruction

bool encode_instruction(char *instruction, symbol_table *symbol_table, word code_image[], long *instruction_counter, data_word data_image[], long *data_counter, AST *AST);

*/

/* This function get a data (imt arry) and convert it to binary code.
@param data - the data to be converted
@param data_image - the array of data words that represent the data
@param data_counter - the number of data words in the code
@param line_counter - the number of the line in the file

void encode_data(int  data[], data_word *data_image, long *data_counter, long line_counter);

*/

/* This function get a string and convert it to binary code. each char is converted to its ascii value.
@param string - the string to be converted
@param data_image - the array of data words that represent the data
@param data_counter - the number of data words in the code
@param line_counter - the number of the line in the file

void encode_string(char *string, data_word *data_image, long *data_counter, long line_counter);

*/
