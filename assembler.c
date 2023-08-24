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

void reset_args(symbol_list *symbol_table, inst_list *instruction_image, data_list *data_image, long *IC, long *DC);


int main(int argc,char *argv[]) { 
    bool generate_files = TRUE; 
    int file_index = 1;
    int file_name_len = 0;
    macro_table *m_table = NULL;
    char *file_name= NULL; 
    long *IC = (long *)INITIAL_INSTRUCTION_COUNTER;
    long *DC = 0;
    inst_list *instruction_image = NULL;
    data_list *data_image = NULL;
    symbol_list *symbol_table;

    while(file_index < argc){
        file_name_len = strlen(argv[file_index]);

        if (file_name_len > MAX_FILE_NAME_LENGTH) {
            printf("File name is too long\n");
            continue;
        }

        strcpy(file_name, argv[file_index]);
        file_name = strcat(file_name, ".as");
        m_table = macro_table_init();
        if(!pre_assembler(file_name, m_table)){
            generate_files = FALSE; 
        }
        free_macro_table(m_table);
        
        if (!generate_files) {
            file_index++;
            continue;
        }
        /*First pass of the assembler*/

        symbol_table = init_symbol_list();
        /*Init the instruction image (an arry of pionters to inst_word)*/
        
        data_image = init_data_list();        
        instruction_image = init_inst_list();

        generate_files = parse(file_name, symbol_table,DC,IC,data_image,instruction_image); 

        if(generate_files){
            generate_all_files(file_name, symbol_table, instruction_image, data_image, IC, DC);
        }
        reset_args(symbol_table, instruction_image, data_image, IC, DC);
        file_index++;
    }
    /*Free all the memory*/
    
    return 0;
}

void reset_args(symbol_list *symbol_table, inst_list *instruction_image, data_list *data_image, long *IC, long *DC){
    free_symbol_list(symbol_table);
    free_inst_list(instruction_image);
    free_data_list(data_image);
    *IC = INITIAL_INSTRUCTION_COUNTER;
    *DC = 0;
}