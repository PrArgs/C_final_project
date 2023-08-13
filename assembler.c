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



int main(int args, char *argv[]){
    bool generate_files = TRUE;
    int i = 1;
    macro_table *macro_table = macro_table_init();
    symbol_list *symbol_table = symbol_list_init();
    int *IC = INITIAL_INSTRUCTION_COUNTER;
    int *DC = 0;

    while(i < args){
        char *file_name = strcat(argv[i], ".as");
        if(!pre_assembler((argv[i]), macro_table)){
            generate_files = FALSE;
        }
        if(!first_parse(file_name, symbol_table, DC, IC)){
            generate_files = FALSE;
        }
        
    
        i++;
    }

}