#include "nachine_code.h"

#define ALL_RIGHT_BIT_ON 63


char *encode_to_sixf(word *bin_filed){

    unsigned i = word;
    unsigned right = 0;
    unsigned left = 0;
    unsigned mask = ALL_RIGHT_ON;
    char *result[2]="";
    
    right = bin_filed & mask;
    mask = mask << 6;
    left = bin_filed & mask;
    left = left >> 6;
    *result[0] = '0' + left;
    *result[1] = '0' + right;
    
    return result;    
}

char int_to_sixf(int *num){
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