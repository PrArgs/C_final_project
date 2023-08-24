/*this File declare the function of the util.c 
the fuctions are base converters
printers
distructors (freeing memory by type)
*/
#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*
takes arbitrary number of pionters and free the momory they point to
@ num: number of pointers
@ ...: pointers
*/



/*in the assembller the names are often are the second word in the line so this function returns the second word in the line for this purpose
@line: the line to get the second word from
@returns: the second word in the line
*/
char *get_second_word(char *line){
    char *second_word;
    second_word = strtok(line, " ");
    second_word = strtok(NULL, " ");
    return second_word;
}

char *remove_first_word(char *current_line);

char *toLowerCase(char *str);

int complement_2(int num);

char *encode_to_sixf(unsigned int bin_filed);

char int_to_sixf(const int num);

#endif