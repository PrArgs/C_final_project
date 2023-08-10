# include "data_structures.h"



/*This is the decleration of function wiil add a new data to the list (each node in the list is unique)
@params list: a pointer to the list
@params data: the data to add
@retuns : true if the data was added successfully and false otherwise
*/
bool add_to_list(list *list, char *data);

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

node *node_init(char *data){
    node *node = malloc(sizeof(node));
    if (!node){
        exit(1);/*exit if malloc fails*/
    }
    char *str = malloc(sizeof(char) * MAX_LINE_LENGTH);
    if (!str){
        exit(1);/*exit if malloc fails*/
    }
    strcpy(str, data);
    node->data = str;
    node->next = NULL;
    return node;
}

list *list_init(char *data)
{  
    list *list = malloc(sizeof(list));
    if (!list){
       exit(1);/*exit if malloc fails*/
    }
    node *head = node_init(data);
    list->head = head;
}

bool list_add(list *list, node *new_node){
    if (!list->head){
        list->head = new_node;
        return TRUE;
    }
    else{
        node *current = list->head;
        node *previous = NULL;
        while(current){     
            previous = current;
            current = current->next;
        }
        previous->next = new_node;
        return TRUE;
    }
    return FALSE;
}

bool list_remove(list *list, char *data){
    node *current = list->head;
    node *previous = NULL;
    while(current){
        if (strcmp(current->data, data) == 0){
            if (!previous){ /*if the node to be removed is the head*/
                list->head = current->next;
            }
            else
            {
                previous->next = current->next;
            }
            free(current);
            return TRUE;
        }
        previous = current;
        current = current->next;
    }
    return FALSE;
}
bool list_contains(list *list, char *data){
    node *current = list->head;
    while(current){
        if (strcmp(current->data, data) == 0){
            return TRUE;
        }
        current = current->next;
    }
    return FALSE;
}
 
bool list_free(list *list){
    node *current = list->head;
    node *next = NULL;
    while(current){
        next = current->next;
        free(current);
        current = next;
    }
    free(list);
    return TRUE;
}





