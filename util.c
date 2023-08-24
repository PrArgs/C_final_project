#include "util.h"
#define ALL_RIGHT_BIT_ON 63


char *encode_to_sixf(unsigned int bin_filed){

    unsigned i = bin_filed;
    unsigned right = 0;
    unsigned left = 0;
    unsigned mask = ALL_RIGHT_BIT_ON;
    char *result[3]="12\n";
    
    right = i & mask;
    mask = mask << 6;
    left = i & mask;
    left = left >> 6;
    *result[0] = int_to_sixf(left);
    *result[1] = int_to_sixf(right);
    return result;    
}

char int_to_sixf(const int num){
    if(num <0){
        printf("Error: negative number can't be encoded to 64 base\n");
        exit(1);
    }

    if(num < 26){
        return 'A' + num;
    }
    else if(num < 52){
        return 'a' + num - 26;
    }
    else if(num < 62){
        return '0' + num - 52;
    }
    else if(num == 62){
        return '+';
    }
    else if(num == 63){
        return '/';
    }
    else{
        printf("Error: number %d can't be encoded to 64 base\n", num);
        exit(1);
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


