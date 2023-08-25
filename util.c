#include "util.h"
#define ALL_RIGHT_BIT_ON 63

void get_second_word(char *line , char *result){
    strcpy(result,line);
    result = strtok(result, " ");
    result = strtok(NULL, " ");
}


void encode_to_sixf(unsigned int bin_filed , char *result){

    unsigned i = bin_filed;
    unsigned right = 0;
    unsigned left = 0;
    unsigned mask = ALL_RIGHT_BIT_ON;
    strcpy(result, "ER\n");
    
    right = i & mask;
    mask = mask << 6;
    left = i & mask;
    left = left >> 6;
    result[0] = (char)int_to_sixf(left);
    result[1] = (char)int_to_sixf(right);
    strcat(result, "\n");
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

void get_first_word(char *line , char *result){
    strcpy(result,line);
    strcpy(result,strtok(result, " "));
}

void remove_first_word(char *current_line , char *result){
    bool read_first = FALSE, last_was_space = TRUE , word_exists = FALSE;
    int i = 0;
    while (current_line[i] != '\n'){            
        if(!(isspace(current_line[i]))){

            if(read_first){
                strcpy(result, (current_line+i));
                word_exists = TRUE;
                break;
            }
            else{
                last_was_space = FALSE;
            }
        }
        else{
            if(!last_was_space)
            {
                last_was_space = TRUE;
                read_first = TRUE;
            }
        }        
        i++;
    }
    if(!word_exists){
    result = NULL;   
    }              
       
}

char *toLowerCase(char *str){
    char tmp_arr[MAX_LABEL_LENGTH + 1];
    char *result = &tmp_arr[0];
    strcpy((char *)result, str);
    for (int i = 0; result[i]; i++) {
        str[i] = tolower(result[i]);
    }
    return result;
}

int complement_two(int num){
    signed int result = 0;
    signed int mask = 1;
    while (mask)
    {
        result += num ^ mask;/*Flip evry bit one by one*/
        mask <<= 1;
    }
    return result+1;    
}

void under_construction(void){
    printf("                   __                                                                         \n");
    printf("                  /\\ \\                                                                        \n");
    printf(" __  __    ___    \\_\\ \\     __   _ __                                                          \n");
    printf("/\\ \\/\\ \\ /' _ `\\  /'_` \\  /'__`\\/\\`'__\\                                                        \n");
    printf("\\ \\ \\_\\ \\/\\ \\/\\ \\/\\ \\L\\ \\/\\  __/\\ \\ \\/                                                         \n");
    printf(" \\ \\____/\\ \\_\\ \\_\\ \\___,_\\ \\____\\\\ \\_\\                                                         \n");
    printf("  \\/___/  \\/_/\\/_/\\/__,_ /\\/____/ \\/_/                                                         \n");
    printf("                                                                                                \n");
    printf("                                                                                                \n");
    printf("                              __                           __                                 \n");
    printf("                             /\\ \\__                       /\\ \\__  __                          \n");
    printf("  ___    ___     ___     ____\\ \\ ,_\\  _ __   __  __    ___\\ \\ ,_\\/\\_\\    ___     ___          \n");
    printf(" /'___\\ / __`\\ /' _ `\\  /',__\\\\ \\ \\/ /\\`'__\\/\\ \\/\\ \\  /'___\\ \\ \\/\\/\\ \\  / __`\\ /' _ `\\        \n");
    printf("/\\ \\__//\\ \\L\\ \\/\\ \\/\\ \\/\\__, `\\\\ \\ \\_\\ \\ \\/ \\ \\ \\_\\ \\/\\ \\__/\\ \\ \\_\\ \\ \\/\\ \\L\\ \\/\\ \\/\\ \\       \n");
    printf("\\ \\____\\ \\____/\\ \\_\\ \\_\\/\\____/ \\ \\__\\\\ \\_\\  \\ \\____/\\ \\____\\\\ \\__\\\\ \\_\\ \\____/\\ \\_\\ \\_\\      \n");
    printf(" \\/____/\\/___/  \\/_/\\/_/\\/___/   \\/__/ \\/_/   \\/___/  \\/____/ \\/__/ \\/_/\\/___/  \\/_/\\/_/      \n");
    printf("                                                                                                \n");
    printf("                                                                                                \n");
    
} 


