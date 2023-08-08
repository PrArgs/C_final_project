#include "pre_assembler.h"


/* This function will unfold a macros
@param line - a pointer to the line of assembly code
@param table - a pointer to the macro table
@param line_counter - a pointer to the line counter
@return the new line counter
*/

bool unfold_macros(char *line, macro_table *table, int *line_counter);

char *remove_spaces(char *line){
    char *result[MAX_LINE_LENGTH+1] = "";
    int final_length;
    char *current = strtok(line, " ");
    while(current){/*remve exase spaces*/
        strcat(result, current);
        strcat(result, " ");
        current = strtok(NULL, " ");
    }
    /*remove spaces between commas*/
    current = strtok(result, ",");
    if (strlen(current) != strlen(result)){
        char *temp;
        strcpy(temp, current);
        while(current){
            strcat(temp, current);
            strcat(temp, ",");
            current = strtok(NULL, ",");
        }
        strcpy(result, temp);    
    }
    int max = (strlen(result) > MAX_LINE_LENGTH) ? MAX_LINE_LENGTH : strlen(result);
    result[max] = '\n';
    
    return result;
}

char **split_line(char *line, char *delim);

bool can_ignore(char *line){

    char *empty_line= "\n";
    char *first_char = strcat(line, " ");/* Remove spaces from the beginning of the line*/   
    if (first_char[0] == ';' || strcmp(line, empty_line) == 0)
    {
        return TRUE;
    }
    return FALSE;
}



/*This function will add new macro if the line is a macro definition
@param line - a pointer to the line of assembly code
@param table - a pointer to the macro table
@return TRUE if the macro was added, FALSE otherwise
*/ 
bool is_macro(char *line, macro_table *table){
    char *first_word = strtok(line, " ");
    if (strcmp(first_word,"macro") == 0){
        return TRUE;
    }
    return FALSE;
}

/*This function will add new macro if the line is a macro definition
@param line - a pointer to the line of assembly code
@param table - a pointer to the macro table
@return TRUE if the macro was added, FALSE otherwise
*/
bool insert_macro(char *line, macro_table *table,char *macro_name)
{
    while (strcmp("endmcro",line))
    {
        if(!can_ignore(line)){
            char *line = remove_spaces(line);
            /* add line to file and read next line */
        }

        
    }
        

} 
