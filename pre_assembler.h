/* This file diclares the functions that will be used to remove the folowing from the assembly code:
1. Comments
2. Unnecessary spaces
3. Empty lines
4. spread macros
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "ast.h"
#include "table_generator.h"
#include "util.h"


/* This function will convers a single line of assembly code to a string with no extra spaces
@param line - a pointer to the line of assembly code
@return a pointer to the new string
*/
char *remove_spaces(char *line);

/* This function will return a string array of all the elements in the line that are sprated by a given char/string
@param line - a pointer to the line of assembly code
@param delim - the char/string that will be used to split the line
@return a pointer to the new string array
*/
char **split_line(char *line, char *delim);

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

// bool unfold_macros(char *line, macro_table *table, int *line_counter); DEFINE AFTER MACRO TABLE IS DEFINED

/*This function will add new macro if the line is a macro definition
@param line - a pointer to the line of assembly code
@param table - a pointer to the macro table
@return TRUE if the macro was added, FALSE otherwise
*/ 
//bool is_macro(char *line, macro_table *table); // DEFINE AFTER MACRO TABLE IS DEFINED







