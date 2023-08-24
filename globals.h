/* A file storing all the global variables used in the program. */



#ifndef GLOBALS_H
#define GLOBALS_H

#define WORD_SIZE 12 /* The size of a word in the memory. */
#define MAX_FILE_NAME_LENGTH 30 /* The maximum length of a file name. */
#define MAX_POSIBLE_LENGTH 255 /* The maximum length of a line in a file. */
#define MAX_LINE_LENGTH 80 /* The maximum length of a line in the assembly file. */
#define MAX_LABEL_LENGTH 31 /* The maximum length of a label. */
#define MEMORY_SIZE 1023 /* The size of the memory. */





/* Deffing bool type */
typedef enum booleans{ 
    FALSE=0,
    TRUE=1 
    } bool;


#endif