#include "util.h"

void free_theme_all(int num_of_pointers, ...)
{
    int i = num_of_pointers;
    va_list valist;
    va_start(valist, num_of_pointers);
    while (i > 0)
    {
        free(va_arg(valist, void *));/*FIX MEEEEEEEEE*/
        i--;
    }

    
    
}

char *remove_first_word(char *current_line){
    char *result;
    int i;
    while (current_line[i] != ' ')
    {
        if(current_line[i] != '\0'){
            printf("you inserted an invalid command %s\n", current_line);
            exit(1);
        }
        i++;
    }
    result = strcpy(result, current_line + i);
    return result;   
}

char *toLowerCase(char *str){
    char *result;
    result = strcpy(result, str);
    for (int i = 0; result[i]; i++) {
        str[i] = tolower(result[i]);
    }
    return result;
}

int complement_2(int *num){
    signed int result = 0;
    signed int mask = 1;
    while (mask)
    {
        result += *num ^ mask;/*Flip evry bit one by one*/
        mask <<= 1;
    }
    return result+1;
    
    
}