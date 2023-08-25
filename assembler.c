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


void reset_args(symbol_list *symbol_table, inst_list *instruction_image, data_list *data_image, long *instruction_counter, long *data_counter);


int main(int argc,char *argv[]) {
    
    inst_list *instruction_image;
    data_list *data_image;
    symbol_list *symbol_table;   
    macro_table *m_table;   

    
    char *file_name;
    
    int file_index;
    int file_name_len;
    long ic;
    long dc;
    bool generate_files = TRUE; 
    long *IC; /*Since INITIAL_INSTRUCTION_COUNTER is an int we want to avoid garbage values*/
    long *DC;

    instruction_image = NULL;
    data_image = NULL;
    symbol_table = NULL;
    m_table = NULL;

    file_index = 1;
    ic = INITIAL_INSTRUCTION_COUNTER;
    dc = INITIAL_DATA_COUNTER;
    IC = &ic;
    DC = &dc;


    if(argc < 2){
        
        printf("No files were given\n");
        return 0;
    }

    file_name = (char *)malloc(MAX_LINE_LENGTH + 1);/*Over allocate to avoid overflow*/

    while(file_index < argc){
        file_name_len = strlen(argv[file_index]);

        if (file_name_len > MAX_FILE_NAME_LENGTH) {
            printf("File name is too long\n");
            continue;
        }

        strcpy(file_name, argv[file_index]);
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

        strcpy(file_name, argv[file_index]);
        generate_files = parse(file_name, symbol_table,DC,IC,data_image,instruction_image); 

        if(generate_files == TRUE){
            generate_all_files(file_name, symbol_table, instruction_image, data_image, *IC, *DC);
        }
        reset_args(symbol_table, instruction_image, data_image, IC, DC);/*Free all the memory*/
        generate_files = TRUE;
        file_index++;
    }
    free(file_name);
    
    return 1;
}

void reset_args(symbol_list *symbol_table, inst_list *instruction_image, data_list *data_image, long *instruction_counter, long *data_counter){
    long ic, dc;
    free_symbol_list(symbol_table);
    free_inst_list(instruction_image);
    free_data_list(data_image);
    ic = INITIAL_INSTRUCTION_COUNTER;     
    dc = INITIAL_DATA_COUNTER;
    instruction_counter = &ic;
    data_counter = &dc;
}