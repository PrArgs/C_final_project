/*
implementing the following data structures:
3. List
4. Map (Hash Table)
*/


typedef struct node
{
    char *data;
    struct node *next;
} node;

typedef struct list
{
    node *head;
    node *tail;
    int size;
} list;

void list_init(list *list);
void list_add(list *list, char *data);
void list_remove(list *list, char *data);
void list_print(list *list);
void list_free(list *list);


// typedef struct stack
// {
//     list *list;
// } stack;

// typedef struct queue
// {
//     list *list;
// } queue;

/*typedef struct Map
a set of key-value pairs, where each key is a 
  
*/
typedef struct Map
{

} map;






