/* This file diclares the functions that will be used to remove the folowing from the assembly code:
1. Comments
2. Unnecessary spaces
3. Empty lines
4. spread macros
*/
#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "table_generator.h"
#include "util.h"
#include <ctype.h>


/*This is the main function of the pre assembler
@param file_name - the name of the file to pre assemble
@param table - a pointer to the macro table that will be used to unfold and add macros
@return TRUE if the pre assembly was successful, FALSE otherwise
*/
bool pre_assembler(char *file_name, macro_table *table);


/* This function will convers a single line of assembly code to a string with no extra spaces
@param line - a pointer to the line of assembly code
@return a pointer to the new string
*/
char *remove_spaces(char *line);

/* This function will returns TRUE if the line can be ignored e.g. a comment or an empty line
@param line - a pointer to the line of assembly code
@return TRUE if the line is a comment, FALSE otherwise
*/
bool can_ignore(char *line);

/* This function will unfold a macros
@param line - a pointer to the line of assembly code
@param table - a pointer to the macro table
@param line_counter - a pointer to the line counter
@return the new line counter
*/

bool unfold_macros(char *line, macro_table *table, int *line_counter); 

/*This function will check if the line is a macro definition
@param line - a pointer to the line of assembly code
@return TRUE if the line is a macro definition, FALSE otherwise
*/
bool is_macro_definition(char *line);


/*This function will check if the line is a name of a known macro
@param line - a pointer to the line of assembly code
@param table - a pointer to the macro table
@return TRUE if the line is a macro name, FALSE otherwise
*/ 
bool is_macro(char *line, macro_table *table);

/*This function will check if the line is the end of a macro
@param line - a pointer to the line of assembly code
@return TRUE if the line is the end of a macro, FALSE otherwise
*/
bool is_macro_end(char *line);


#endif

