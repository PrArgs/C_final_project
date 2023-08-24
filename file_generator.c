#include "file_generator.h"

void generate_ob_file(char *file_name, long instruction_counter, long data_counter,inst_list *code_image,data_list *data_image){

    char *output_file_name;
    strcpy(output_file_name, file_name);
    strcat(output_file_name, ".ob");
    FILE *output_file = open_file(output_file_name, "w");
    if(output_file == NULL){
        printf("Error: could not open file %s\n", output_file_name);
        exit(1);
    }
    fprintf(output_file, "%ld %ld\n", instruction_counter, data_counter);
    int i = 0;
    char *base64_encoded[3]="";
    unsigned int *bin_filed = 0;

    inst_node *current_inst = get_head(code_image);

    while (i < instruction_counter && current_inst != NULL){
        bin_filed = extract_bits(get_inst_val(current_inst));
        fprintf(output_file, "%s", encode_to_sixf(bin_filed));
        current_inst = get_next(current_inst);
    }
    if(current_inst != NULL){
        printf("Error: there are more instructions than the instruction counter\n");
        exit(1);
    }

    data_node *current_data = get_head(data_image);

    while (i < data_counter && current_data != NULL){
        bin_filed = extract_bits(get_data_val(current_data));
        fprintf(output_file, "%s", encode_to_sixf(bin_filed));
        current_data = get_next(current_data);
    }
    if(current_data != NULL){
        printf("Error: there are more data words than the data counter\n");
        exit(1);
    }

    fclose(output_file);    
}

/* This function generates the .ent file a file that contains the names of the labels that are used as entry points
@ param file_name - the name of the file to be generated
@ param symbol_table - the symbol table
@ return void

*/

void generate_ent_file(char *file_name, symbol_list *symbol_list){
    
    bool have_entry = FALSE;
    char *output_file_name;
    strcpy(output_file_name, file_name);
    strcat(output_file_name, ".ent");
    FILE *output_file = open_file(output_file_name, "w");
    if(output_file == NULL){
        printf("Error: could not open file %s\n", output_file_name);
        exit(1);
    }
    symbol *current_symbol = get_head(symbol_list);
    char *symbol_line = NULL;
    while (current_symbol != NULL){
        if(is_entry(current_symbol)){
            have_entry = TRUE;
            symbol_line = print_symbol(current_symbol);
            fprintf(output_file, "%s", symbol_line);
        }
        current_symbol = get_next(current_symbol);
    }

    fclose(output_file);

    if(!have_entry){/*If no symbol is entry delete the file*/
        remove(output_file_name);
    } 
}


/* This function generates the .ext file a file that contains the names of the labels that are used as external labels
@ param file_name - the name of the file to be generated
@ param symbol_table - the symbol table
@ param code_image - the array of words that represent the instructions
@ param instruction_counter - the number of instructions in the code
*/
void generate_ext_file(char *file_name, symbol_list *symbol_list){
    {
    bool have_external = FALSE;
    char *output_file_name;
    strcpy(output_file_name, file_name);
    strcat(output_file_name, ".ext");
    FILE *output_file = open_file(output_file_name, "w");
    if(output_file == NULL){
        printf("Error: could not open file %s\n", output_file_name);
        exit(1);
    }
    int i = 0;
    char *symbol_line = NULL;
    symbol *current_symbol = get_head(symbol_list);
    while (current_symbol != NULL){
        if(is_extern(current_symbol)){
            have_external = TRUE;
            symbol_line = print_symbol(current_symbol);
            fprintf(output_file, "%s", symbol_line);
        }
        current_symbol = get_next(current_symbol);
    }

    fclose(output_file);

    if(!have_external){/*If no symbol is external delete the file*/
        remove(output_file_name);
    }    

}
}

/* This function generates the files in the case of a valid assembly code.*/
void generate_all_files(char *file_name, symbol_list *symbol_table,inst_list *code_image,data_list *data_image, long instruction_counter, long data_counter){
    generate_ob_file(file_name, instruction_counter, data_counter, code_image, data_image);
    generate_ent_file(file_name, symbol_table);
    generate_ext_file(file_name, symbol_table, code_image, instruction_counter);
}

