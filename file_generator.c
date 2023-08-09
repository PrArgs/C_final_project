/*
This file diclares the functions that will be used to generate the files in the case of a valid assembly code.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util.h"
#include "globals.h"
#include "table_generator.h"

/* This function generates the .ob file 
@ param file_name - the name of the file to be generated
@ param instruction_counter - the number of instructions in the code
@ param data_counter - the number of data words in the code
@ param code_image - the array of words that represent the instructions
@ param data_image - the array of data words that represent the data
*/
void generate_ob_file(char *file_name, long instruction_counter, long data_counter);
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* convert_binary_to_base64(const char* binary_string) {
    int decimal_value = strtol(binary_string, NULL, 2);
    char decimal_string[20]; // Assuming the decimal value won't exceed this
    sprintf(decimal_string, "%d", decimal_value);

    size_t decimal_length = strlen(decimal_string);
    char* base64_encoded = (char*)malloc(decimal_length * 2); // Base64 might take roughly twice the space
    if (base64_encoded == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(1);
    }

    strcpy(base64_encoded, decimal_string);
    return base64_encoded;
}

void process_file(const char* file_name, int instruction_counter, int data_counter) {
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "File not found.\n");
        exit(1);
    }

    char output_file_name[MAX_FILE_NAME_LENGTH];
    snprintf(output_file_name, sizeof(output_file_name), "converted_%s", file_name);
    FILE* output_file = fopen(output_file_name, "w");
    if (output_file == NULL) {
        fprintf(stderr, "Error creating output file.\n");
        exit(1);
    }

    fprintf(output_file, "%d %d\n", instruction_counter, data_counter);

    char line[13];
    while (fgets(line, sizeof(line), file)) {
        line[12] = '\0'; // Ensure null-termination
        char* binary_word = line;

        int decimal_value = strtol(binary_word, NULL, 2);
        char* base64_encoded = convert_binary_to_base64(binary_word);
        
        fprintf(output_file, "%d %s\n", decimal_value, base64_encoded);
        free(base64_encoded);
    }

    fclose(file);
    fclose(output_file);

    printf("Conversion successful. Output written to %s\n", output_file_name);
}

int main() {
    char file_name[MAX_FILE_NAME_LENGTH];
    int instruction_counter, data_counter;

    printf("Enter the file name: ");
    scanf("%s", file_name);
    printf("Enter instruction_counter: ");
    scanf("%d", &instruction_counter);
    printf("Enter data_counter: ");
    scanf("%d", &data_counter);

    process_file(file_name, instruction_counter, data_counter);

    return 0;
}








/* This function generates the .ent file a file that contains the names of the labels that are used as entry points
@ param file_name - the name of the file to be generated
@ param symbol_table - the symbol table

void generate_ent_file(char *file_name, symbol_table *symbol_table);

*/

/* This function generates the .ext file a file that contains the names of the labels that are used as external labels
@ param file_name - the name of the file to be generated
@ param symbol_table - the symbol table
@ param code_image - the array of words that represent the instructions
@ param instruction_counter - the number of instructions in the code

void generate_ext_file(char *file_name, symbol_table *symbol_table, word code_image[], long instruction_counter);

*/
