# include "data_structures.h"

node *node_init(char *data){
    node *node = malloc(sizeof(node));
    if (!node){
        exit(1);/*exit if malloc fails*/
    }    
    strcpy(node->data, data);
    node->next = NULL;
    return node;
}

list *list_init()
{  
    list *list = malloc(sizeof(list));
    if (!list){
       exit(1);/*exit if malloc fails*/
    }
    list->head = NULL;
}

bool add_to_list(list *list, node *new_node){
    if (!list->head){/*if the list is empty*/
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

bool list_remove(list *list, node *r_node){
    node *current = list->head;
    node *previous = NULL;
    while(current){
        if (strcmp(current->data, r_node->data) == 0){
            if (previous){/*if the node is not the first node in the list*/
                previous->next = current->next;
            }
            else{
                list->head = current->next;
            }            
            free(current);
            return TRUE;
        }
        previous = current;
        current = current->next;
    }
}

bool list_contains(list *list,  node *node_to_search){
    node *current = list->head;
    while(current){
        if (strcmp(current->data, node_to_search->data) == 0){
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

void print_list(list *list){
    node *current = list->head;
    while(current){
        printf("%s\n", current->data);
        current = current->next;
    }
}



