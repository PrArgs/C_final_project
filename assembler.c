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

        generate_files = first_parse(file_name, symbol_table, DC, IC,data_image,data_image); // pass pointers to DC and IC

        
        if(generate_files){
            /*Second pass of the assembler*/
            generate_files = second_parse(file_name, symbol_table, IC);
        }

        file_index++;
    }
    return 0;
}