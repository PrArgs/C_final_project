#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "data_structures.h"
#include "table_generator.h"
#include "file_generator.h"
#include "pre_assembler.h"
#include "ast.h"
#include "parser.h"
#include "util.h"


/*Global variables*/
char INPUT_FILE_TYPE[3] = ".as";


int main(int args, char *argv[]){
    int i = 1;
    macro_table *macro_table = macro_table_init();

    while(i < args){
        char *file_name = strcat(argv[i], ".as");
        pre_assembler((argv[i]), macro_table);
        i++;
    }

}