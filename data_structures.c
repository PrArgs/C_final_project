# include "data_structures.h"

list *list_init(char *data)
{  
    list *list = malloc(sizeof(list));
    if (!list){
       exit(1);/*exit if malloc fails*/
    }
    list->head = malloc(sizeof(node));
    if (!list->head){/*exit if malloc fails*/
        exit(1);
    }
    list->head->data = data;
    list->head->next = NULL;
    return list;
}

bool list_add(list *list, char *data){
    node *current = list->head;
    while(current->next){
        if (strcmp(current->data, data) == 0){
            printf("data already exists\n");
            return FALSE;/*return false if data already exists*/
        }
        current = current->next;
    }
    current->next = malloc(sizeof(node));
    if (current->next == NULL){/*return false if malloc fails*/
        return FALSE;
    }
    current->next->data = data;
    current->next->next = NULL;
    return TRUE;
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





