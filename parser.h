/*This is the final procces of the assembler. It will read all the line of the clean .as file and will build the following:

1. Symbol table
2. Data table
3. Code table
4. Entry table
5. Extern table
6. data image
7. instruction image


or an .as file with all the errors that were found in the pre assembling and parsering procceses
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "ast.h"
#include "table_generator.h"
#include "util.h"
#include "machine_code.h"


/*This function will iterate over the lines of the .as file for the first time in order to build the following:
1. Symbol table
2. Data image
3. Code image
4. Entry table
5. Extern table
in addition it will check for errors in the .as file and print them to the same file

@param line - the line of the .as file
@param symbol_table - the symbol table
@param data_image - the data image
@param instruction_image - the instruction image
@param data_counter - the data counter
@param instruction_counter - the instruction counter
@return - true if the line is valid, false otherwise

*/

bool first_parse(char *line, symbol_list *symbol_table, data_word *data_image, instrction_word *instruction_image, int *data_counter, int *instruction_counter); 

/*This function will iterate over the lines of the .as file for the second time in order to:
1. inset missing data to instruction image
2. insert missing data to data image
3. find additional errors in the .as file and print them to the same file

@param line - the line of the .as file
@param symbol_table - the symbol
@param AST - the AST of the line
@param code_image - the code image
@param code_counter - the code counter
@return - true if the line is valid, false otherwise

*/

/*bool second_parse(char *line, symbol_table *symbol_table, AST *AST, code_image *code_image, int *code_counter); 

                ***---------IMPLEMET AFTER THE DATA STRUCTURES ARE DONE----------***              
                */

