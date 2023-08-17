#include "pre_assembler.h"

bool pre_assembler(char *file_name, macro_table *table){
    
    bool result = TRUE;
    bool read_macro = FALSE; /*a flag that will be true if the line is a macro definition*/
    int line_counter = 0;
    char *macro_name[MAX_LINE_LENGTH+1];/*the name of potential macro*/
    strcpy(macro_name, "");
    list *macro_lines; /*a pionte that will be used to print the macro*/
    char line[MAX_LINE_LENGTH+1] = "";

    /*Setting the macro table and files*/
    macro_table *macro_table = macro_table_init();/*create a macro table*/
    if (!macro_table){
        printf("Error: macro table could not be created\n");
        exit(1);
    }
    FILE *file = fopen(file_name, "r");
    if(!file){
        printf("Error: file %s does not exist\n", file_name);
        return FALSE;
    }
    /*creat the .as file*/
    char *file_name_as = strcat(strtok(file_name,"."), ".am");
    FILE *file_as = fopen(file_name_as, "w");
    if(!file_as){
        printf("Error: file %s could not be created\n", file_name_as);
        return FALSE;
    }

    
    /*iterate over the file*/
    while(fgets(line, MAX_LINE_LENGTH, file)){
        if(!can_ignore(line)){
            strcpy(line, remove_spaces(line));
            if(is_macro_definition(line))
            {
                read_macro = TRUE;
                strcpy(macro_name, get_second_word(line));
                if(!(add_new_macro(macro_table,macro_name)))
                {
                    fprintf(file_as, "Error: macro %s already exists \n",macro_name);
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
                    add_to_macro(macro_table, line, macro_name);
                }
            }
            else if(is_macro(line, macro_table)){
                macro_lines = get_macro_lines(macro_table,macro_name);
                node *current = macro_lines->head;
                while(current){
                    fprintf(file_as, strcat(current->data, "\n"));
                    current = current->next;
                    line_counter++;
                }
            }  
            else{/*if the line is not a macro a macro or a part of a macro*/
                fprintf(file_as, strcat(line, "\n"));
                line_counter++;              
            }
        }/*end of can't be ignored*/
        if (line_counter > MEMORY_SIZE){
            fprintf(file_as, "Error: file is too long\n");
            result = FALSE;
        }        
    }

    /*free all alocaed memory*/
    fclose(file);
    fclose(file_as);
    free_macro_table(macro_table);
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

bool can_ignore(char *line){
    char *empty_line= "\n";
    char *first_char = strcat(line, " ");/* Remove spaces from the beginning of the line*/   
    if (first_char[0] == ';' || strcmp(line, empty_line) == 0)
    {
        return TRUE;
    }
    return FALSE;
}

/*###########################
#### Might be useless  ######
#############################
*/
bool unfold_macros(char *line, macro_table *table, int *line_counter)
{
    bool result = TRUE;
    return result;
}

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
    if (strcmp(line, "endmcro") == 0)
    {
        return TRUE;
    }
    return FALSE;
}
