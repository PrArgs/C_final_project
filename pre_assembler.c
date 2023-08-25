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
    FILE *file_am;
    FILE *file;

    macro_name = (char *)malloc(MAX_LINE_LENGTH+1);
    error_msg = (char *)malloc(MAX_POSIBLE_LENGTH);
    file_name_am = (char *)malloc(MAX_FILE_NAME_LENGTH + strlen(".am") + 1);
    line = (char *)malloc(MAX_POSIBLE_LENGTH);
    
    strcpy(file_name_am, file_name);    
    strcat(file_name, ".as");    
    strcpy(line, "");
    strcpy(error_msg, "");

    


    file = fopen(file_name, "r");
    if(!file){        
        printf("Error: file %s does not exist\n", file_name);
        free(macro_name);
        free(error_msg);
        free(file_name_am);
        free(line);
        return FALSE;
    }

    strcat(file_name_am, ".am");
    file_am = fopen(file_name_am, "w");

    if(!file_am){
        printf("Error: file %s could not be created\n", file_name_am);
         free(macro_name);
        free(error_msg);
        free(file_name_am);
        free(line);
        return FALSE;
    }

    
    /*iterate over the file*/
    while(fgets(line, MAX_LINE_LENGTH+1, file)){        
        if(!can_ignore(line)){
            if(is_macro_definition(line))
            {
                if(read_macro){/*if there is already a macro being defined*/
                    fprintf(file_am, "Error at line %d: macro definition inside a macro definition\n", line_counter);
                    result = FALSE;
                }
                read_macro = TRUE;
                get_second_word(line,macro_name);
                if(!(add_new_macro(table,macro_name,error_msg)))/*Macro already exists or too long name*/
                {
                    fprintf(file_am,"%s",error_msg);
                    strcpy(error_msg, "");
                    result = FALSE;
                }
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
                fprintf(file_am, "%s", line);                            
            }          
        }     
    }

    /*free all alocaed memory*/
    fclose(file);
    fclose(file_am);
    free(macro_name);
    free(error_msg);
    free(file_name_am);
    free(line);
    return result;
}



bool can_ignore(char *line) {
    while (*line) {
        if (*line == ';') {
            return TRUE; 
        }
        else if (!isspace((unsigned char)*line))/*if char is not a blank we should read the line*/
        {
            return FALSE; 
        }        
        *line++;/*move to the next char of line*/
    }
    return FALSE; /* Line is blank*/
}

bool is_macro_definition(char *line)
{   char f_w[MAX_LINE_LENGTH + 1];
    char *first_word = &f_w[0];
    get_first_word(line, first_word);

    if (strcmp(first_word, "macro") == 0)
    {
        return TRUE;
    }
    return FALSE;
}

bool is_macro(char *line, macro_table *table){
    char original_line[MAX_LINE_LENGTH + 1];
    char *first_word;
    first_word = &original_line[0];
    get_first_word(line, first_word);
    return table_contains(table, first_word);
    
}

bool is_macro_end(char *line){
    char og_line[MAX_LINE_LENGTH + 1];
    get_first_word(line, (char *)og_line);
    if(strcmp(og_line, "endmcro") == 0)
    {
        return TRUE;
    }
    return FALSE;
}
