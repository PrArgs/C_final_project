#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h" 
#include "data_structures.h"
#include "table_generator.h"
#include "file_generator.h"
#include "pre_assembler.h"
#include "ast.h"
#include "parser.h"
#include "util.h"

#define INITIAL_INSTRUCTION_COUNTER 100 /* The piont in memory where the code begins. */

void reset_args(symbol_list *symbol_table, instruction_word **instruction_image, data_word **data_image, long *IC, long *DC)


int main(int argc,char *argv[]) { 
    bool generate_files = TRUE; 
    int file_index = 1;
    macro_table *m_table = macro_table_init();
    char *file_name = malloc(sizeof(char) * 100); // allocate memory for file_name
    long *IC = INITIAL_INSTRUCTION_COUNTER; // remove pointer
    long *DC = 0;

    while(file_index < argc){

        strcpy(file_name, argv[file_index]);
        file_name = strcat(file_name, ".as");
        if(!pre_assembler(file_name, m_table)){
            generate_files = FALSE; // change FALSE to false
        }
        if (m_table != NULL) {
            printf("Somthing went wrong with the pre-assembler\n");
            free_macro_table(m_table);
        }

        /*First pass of the assembler*/

        symbol_list *symbol_table = init_symbol_list();
        /*Init the instruction image (an arry of pionters to inst_word)*/
        instruction_word **instruction_image[] = malloc(sizeof(instruction_word*) * MEMORY_SIZE);
        /*Init data image (an arry of pionters to data_word)*/
        data_word **data_image[] = malloc(sizeof(data_word*) * MEMORY_SIZE);

        generate_files = first_parse(file_name, symbol_table, DC, IC,data_image,data_image); 

        if(generate_files){
            generate_files(file_name, symbol_table, instruction_image, data_image, IC, DC);
        }
        reset_args(symbol_table, instruction_image, data_image, IC, DC);
        file_index++;
    }
    return 0;
}

void reset_args(symbol_list *symbol_table, instruction_word **instruction_image, data_word **data_image, long *IC, long *DC){
    free_symbol_list(symbol_table);
    free_ins_array(instruction_image, *IC);
    free_data_array(data_image, *DC);
    *IC = INITIAL_INSTRUCTION_COUNTER;
    *DC = 0;
}