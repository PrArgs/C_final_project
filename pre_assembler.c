#include "pre_assembler.h"

bool pre_assembler(char *file_name, macro_table *table){
    
    bool result = TRUE;
    bool read_macro = FALSE; /*a flag that will be true if the line is a macro definition*/
    int line_counter = 0;
    char *macro_name;/*the name of potential macro*/
    char *error_msg;
    char *file_name_am;
    char *line;/*the line that will be read from the file*/
    list *macro_lines = NULL; /*a pionte that will be used to print the macro*/
    FILE *file_as;

    macro_name = (char *)malloc(MAX_LINE_LENGTH+1);
    error_msg = (char *)malloc(MAX_POSIBLE_LENGTH);
    file_name_am = (char *)malloc(MAX_FILE_NAME_LENGTH + strlen(".am") + 1);
    line = (char *)malloc(MAX_POSIBLE_LENGTH);


    strcat(strtok(file_name,'.'), ".am");    
    strcpy(line, "");
    strcpy(error_msg, "");

    FILE *file = fopen(file_name, "r");
    if(!file){
        printf("Error: file %s does not exist\n", file_name);
        free(macro_name);
        free(error_msg);
        free(file_name_am);
        free(line);
        return FALSE;
    }

    strcpy(file_name_am, file_name);
    strcat(file_name_am, ".am");
    file_as = fopen(file_name_am, "w");

    if(!file_as){
        printf("Error: file %s could not be created\n", file_name_am);
         free(macro_name);
        free(error_msg);
        free(file_name_am);
        free(line);
        return FALSE;
    }

    
    /*iterate over the file*/
    while(fgets(line, MAX_POSIBLE_LENGTH, file)){
        if(!can_ignore(line)){
            if(is_macro_definition(line))
            {
                if(read_macro){/*if there is already a macro being defined*/
                    fprintf(file_as, "Error: macro definition inside a macro is not allowed\n");
                    result = FALSE;
                }
                read_macro = TRUE;
                strcpy(macro_name, get_second_word(line));
                if(!(add_new_macro(table,macro_name,error_msg)))/*Macro already exists or too long name*/
                {
                    fprintf(file_as,"%s",error_msg);
                    strcpy(error_msg, "");
                    result = FALSE;
                };
            }
            else if(read_macro){                
                if (is_macro_end(line))/*Stop reading and defining macro*/
                {
                    strcpy(macro_name, "");
                    read_macro = FALSE;
                }
                else
                {
                    add_to_macro(table, line, macro_name,error_msg);
                }
            }
            else{/*if the line is not a macro a macro or a part of a macro*/
                fprintf(file_as, "%s", line);                            
            }
            line_counter++;
        }/*end of can't be ignored*/
        
        if (line_counter > MEMORY_SIZE){
            fprintf(file_as, "Error: file is too long\n");
            result = FALSE;
        }        
    }

    /*free all alocaed memory*/
    fclose(file);
    fclose(file_as);
    free(macro_name);
    free(error_msg);
    free(file_name_am);
    free(line);
    return result;
}

char *remove_spaces(char *line){
    char *result[MAX_LINE_LENGTH+1];
    strcpy(result, "");
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
    return result;
}

bool can_ignore(const char *line) {
    while (*line) {
        if (*line == ';') {
            return TRUE; 
        }
        else if (!isspace((unsigned char)*line))/*if char is not a blank we should read the line*/
        {
            return FALSE; 
        }
        line++;/*move to the next char of line*/
    }
    return FALSE; // Line is blank
}

/*###########################
#### Might be useless  ######
#############################

bool unfold_macros(char *line, macro_table *table, int line_counter)
{
    bool result = TRUE;
    return result;
}
*/

bool is_macro_definition(char *line)
{
    char *first_word = strtok(line, " ");
    if (strcmp(first_word, "macro") == 0)
    {
        char *macro_name = get_second_word(line);
    }
}

bool is_macro(char *line, macro_table *table){
    char *first_word = strtok(line, " ");
    return table_contains(table, first_word);
    
}

bool is_macro_end(char *line){
    line = strtok(line, '\n');
    if(strcmp(line, "endmcro") == 0)
    {
        return TRUE;
    }
    return FALSE;
}
