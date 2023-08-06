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
    char *data;
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



/*Our "special" map is a hasj table the data is stored in an array of lists
@params:
    size: size of the array
    array: the array of lists 
    hash_function: a function that returns the index of the list in which the data should be stored
    the default hash function is the sum of the ascii values of the data characters
    compare_function: a function that compares two data elements and returns 1 if they are equal and 0 otherwise
*/

typedef struct map
{
    int size=MEMORY_SIZE; //Default size of the map
    list* arry[MEMORY_SIZE];//array of lists
    int (*hash_function)(char *,int arry_size) = *default_hash_function;
} map;

/* Hash function that returns the sum of 
the multiplication of (the index of the character in the string) and (the ascii value of the character)
modulo the size of the map

@params data: the data to hash

*/
int default_hash_function(char *data,int arry_size)
{

    if (!arry_size)
    {
        arry_size=MEMORY_SIZE;
    }
    int sum = 0, i=0,u=0;
    for (; i < strlen(data); i++)
    {
        sum += i*data[i];
    }
    return sum%arry_size;
}

bool default_compare_function(char *data1, char *data2)
{
    return strcmp(data1, data2) == 0;
}



/*This is the decleration of function wiil init a new hash table
@params size: size of the array
@params hash_function: a function that returns the index of the list in which the data should be stored
@params compare_function: a function that compares two data elements and returns 1 if they are equal and 0 otherwise
@returns: a pointer to the new hash table
*/
map *map_init(int size, int (*hash_function)(char *,int arry_size), bool (*compare_function)(char *, char *));

/*This is the decleration of function wiil add a new data to the hash table
@params map: a pointer to the hash table
@params data: the data to add
@returns: 1 if the data was added successfully and 0 otherwise
*/
bool map_add(map *map, char *data);

/*This is the decleration of function wiil remove a data from the hash table
@params map: a pointer to the hash table
@params data: the data to remove
@returns: 1 if the data was removed successfully and 0 otherwise
*/
bool map_remove(map *map, char *data);

/*This is the decleration of function wiil check if a data is in the hash table 
@params map: a pointer to the hash table
@params data: the data to check
@returns: 1 if the data is in the hash table and 0 otherwise
*/
bool map_contains(map *map, char *data);

/*This is the decleration of function wiil free the memory of the hash table
@params map: a pointer to the hash table
@returns: true if the data was removed successfully and false otherwise
*/
bool map_free(map *map);





