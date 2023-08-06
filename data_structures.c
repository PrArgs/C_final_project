# include "data_structures.h"

list *list_init(char *data)
{  
    list *list = malloc(sizeof(list));
    if (list == NULL){
       exit(1);/*exit if malloc fails*/
    }
    list->head = malloc(sizeof(node));
    if (list->head == NULL){/*exit if malloc fails*/
        exit(1);
    }
    list->head->data = data;
    list->head->next = NULL;
    return list;
}

bool list_add(list *list, char *data){
    node *current = list->head;
    while(current->next != NULL){
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
    while(current != NULL){
        if (strcmp(current->data, data) == 0){
            if (previous == NULL){ /*if the node to be removed is the head*/
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
bool list_contains(list *list, char *data);
bool list_free(list *list);
map *map_init(int size, int (*hash_function)(char *,int arry_size), bool (*compare_function)(char *, char *));
bool map_add(map *map, char *data);
bool map_remove(map *map, char *data);
bool map_contains(map *map, char *data);
bool map_free(map *map);






