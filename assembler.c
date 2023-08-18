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

int main(int argc,char *argv[]) { 
    bool generate_files = TRUE; 
    int file_index = 1;
    macro_table *m_table = macro_table_init();
    char *file_name = malloc(sizeof(char) * 100); // allocate memory for file_name
    int IC = INITIAL_INSTRUCTION_COUNTER; // remove pointer
    int DC = 0;

    while(file_index < argc){

        strcpy(file_name, argv[file_index]);
        file_name = strcat(file_name, ".as");
        if(!pre_assembler(file_name, m_table)){
            generate_files = FALSE; // change FALSE to false
        }

        /*First pass of the assembler*/

        symbol_list *symbol_table = init_symbol_list();


        if(!first_parse(file_name, symbol_table, &DC, &IC)){ // pass pointers to DC and IC
            // handle error
        }

        file_index++;
    }
    return 0;
}