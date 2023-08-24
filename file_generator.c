#include "file_generator.h"

void generate_ob_file(char *file_name, long instruction_counter, long data_counter,inst_list *code_image,data_list *data_image){

    char *output_file_name;
    int i = 0;
    unsigned int bin_filed = 0;
    char *base64_encoded;
    inst_node *current_inst;
    data_node *current_data;
    FILE *file;

    output_file_name = (char *)malloc(strlen(file_name) + strlen(".ob") + 1);
    if (output_file_name == NULL) {
        printf("Error: memory allocation failed.\n");
        free(output_file_name);
        exit(1);
    }

    strcpy(output_file_name, file_name);
    strcat(output_file_name, ".ob");
    file = open_file(output_file_name, "w");
    if(file == NULL){
        printf("Error: could not open file %s\n", output_file_name);
        free(output_file_name);
        exit(1);
    }

    fprintf(file, "%ld %ld\n", instruction_counter, data_counter);
    current_inst = get_head_inst(code_image);

    while (i < instruction_counter && current_inst != NULL){
        bin_filed = get_inst_val(current_inst);
        fprintf(file, "%s", encode_to_sixf(bin_filed));
        current_inst = get_next_inst(current_inst);
    }
    if(current_inst != NULL){
        printf("Error: there are more instructions than the instruction counter\n");
        exit(1);
    }

    current_data = get_head_data(data_image);

    while (i < data_counter && current_data != NULL){
        bin_filed = get_data_val(current_data);
        fprintf(file, "%s", encode_to_sixf(bin_filed));
        current_data = get_next_data(current_data);
    }
    if(current_data != NULL){
        printf("Error: there are more data words than the data counter\n");
        exit(1);
    }
    free(output_file_name);
    fclose(file);    
}

/* This function generates the .ent file a file that contains the names of the labels that are used as entry points
@ param file_name - the name of the file to be generated
@ param symbol_table - the symbol table
@ return void

*/

void generate_ent_file(char *file_name, symbol_list *symbol_list){
    
    bool have_entry = FALSE;
    symbol *current_symbol;
    char *symbol_line = NULL;
    char *output_file_name;
    FILE *file;

    output_file_name = (char *)malloc(strlen(file_name) + strlen(".ent") + 1);
    strcpy(output_file_name, file_name);
    strcat(output_file_name, ".ent");
    file = open_file(output_file_name, "w");
    if(file == NULL){
        printf("Error: could not open file %s\n", output_file_name);
        free(output_file_name);
        exit(1);
    }
    
    current_symbol = get_list_head(symbol_list);
    while (current_symbol != NULL){
        if(is_entry_s(current_symbol)){
            have_entry = TRUE;
            symbol_line = print_symbol(current_symbol);
            fprintf(file, "%s", symbol_line);
        }
        current_symbol = get_next_symbol(current_symbol);
    }

    fclose(file);

    if(!have_entry){/*If no symbol is entry delete the file*/
        remove(output_file_name);
    }
    free(output_file_name); 
}


/* This function generates the .ext file a file that contains the names of the labels that are used as external labels
@ param file_name - the name of the file to be generated
@ param symbol_table - the symbol table
@ param code_image - the array of words that represent the instructions
@ param instruction_counter - the number of instructions in the code
*/
void generate_ext_file(char *file_name, symbol_list *symbol_list){

    int i = 0;
    char *symbol_line = NULL;
    symbol *current_symbol;
    bool have_external = FALSE;
    char *output_file_name;
    FILE *file;

    output_file_name = (char *)malloc(strlen(file_name) + strlen(".ext") + 1);
    strcpy(output_file_name, file_name);
    strcat(output_file_name, ".ext");
    file = open_file(output_file_name, "w");
    if(file == NULL){
        printf("Error: could not open file %s\n", output_file_name);
        free(output_file_name);
        exit(1);
    }

    current_symbol = get_head(symbol_list);
    while (current_symbol != NULL){
        if(is_extern_s(current_symbol)){
            have_external = TRUE;
            symbol_line = print_symbol(current_symbol);
            fprintf(file, "%s", symbol_line);
        }
        current_symbol = get_next(current_symbol);
    }
    
    fclose(file);
    free(output_file_name);

    if(!have_external){/*If no symbol is external delete the file*/
        remove(output_file_name);
    }   
}

/* This function generates the files in the case of a valid assembly code.*/
void generate_all_files(char *file_name, symbol_list *symbol_table,inst_list *code_image,data_list *data_image, long instruction_counter, long data_counter){
    generate_ob_file(file_name, instruction_counter, data_counter, code_image, data_image);
    generate_ent_file(file_name, symbol_table);
    generate_ext_file(file_name, symbol_table);
}

