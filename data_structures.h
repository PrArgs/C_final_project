/*
implementing the following data structures:
3. List
4. Map (Hash Table)
*/
#include <stdlib.h>
#include <string.h>
#include "globals.h"


typedef struct node
{
    char *data[MAX_LINE_LENGTH];
    struct node *next;
    
} node;

typedef struct list
{
    node *head;    
} list;

/*This is the decleration of function wiil init a new list
@params data: a pointer to the data to store in the first node of the list
@returns: a pointer to the new list
*/
list *list_init(char *data);

/*This is the decleration of function wiil add a new data to the list (each node in the list is unique)
@params list: a pointer to the list
@params data: the data to add
@retuns : true if the data was added successfully and false otherwise
*/
bool list_add(list *list, char *data);

/*This is the decleration of function wiil remove a data from the list
@params list: a pointer to the list
@params data: the data to remove
@returns: true if the data was removed successfully and false otherwise
*/
bool list_remove(list *list, char *data);

/*This is the decleration of function wiil check if a data is in the list
@params list: a pointer to the list
@params data: the data to check
@returns: true if the data is in the list and false otherwise
*/
bool list_contains(list *list, char *data);

/*This is the decleration of function wiil free the list
@params list: a pointer to the list
@returns: true if the list was freed successfully and false otherwise
*/
bool list_free(list *list);





