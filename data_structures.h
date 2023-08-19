/*
implementing the following data structures:
3. List
4. Map (Hash Table)
*/
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H
#include <stdlib.h>
#include <stdio.h>
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

/*This function will init a new node
@params data: a pointer to the data to store in the node
@returns: a pointer to the new node
*/
node *node_init(char *data);

/*This is the decleration of function wiil init a new list
@returns: a pointer to the new list
*/
list *list_init();

/*This is the decleration of function wiil add a new data to the list (each node in the list is unique)
@params list: a pointer to the list
@params data: the data to add
@retuns : true if the data was added successfully and false otherwise
*/
bool add_to_list(list *list, node *new_node);

/*This is the decleration of function wiil remove a data from the list
@params list: a pointer to the list
@params data: the data to remove
@returns: true if the data was removed successfully and false otherwise
*/
bool list_remove(list *list, node *r_node);

/*This is the decleration of function wiil check if a data is in the list
@params list: a pointer to the list
@params data: the data to check
@returns: true if the data is in the list and false otherwise
*/
bool list_contains(list *list, node *node_to_search);

/*This is the decleration of function wiil free the list
@params list: a pointer to the list
@returns: true if the list was freed successfully and false otherwise
*/
bool list_free(list *list);

/*This function will print the list
@params list: a pointer to the list
*/
void print_list(list *list);





#endif