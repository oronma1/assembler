#ifndef _LIST_
#define _LIST_



typedef struct node
{
    void* data;
    struct node* next;
} Node;



typedef struct {
    Node* head;
}List;




/* @ Function: start_list
   @ Arguments: This function gets List* list 
   @ Description: The function initializes the linked list (head=NULL)
   */
void start_list(List* list);

/* @ Function: get_from_list
   @ Arguments: This function gets List* list, void* data - The type of values the linked list holds
   int(*compre)(void*, void*))- A comparison function for the type of values that the linked list contains
   @ Description: The function searches the linked list 
   for the DATA and returns a pointer to it 
   if it exists, otherwise returns a pointer to NULL
   */

void* get_from_list(List* list, void* data, int(*compre)(void*, void*));

/* @ Function: insert_to_list
   @ Arguments: This function gets List* list, void* data-The type of values the linked list holds
   int(*compre)(void*, void*))-A comparison function for the type of values that the linked list contains
   @ Description: The function checks if DATA already exists in the list, if so it returns 0.
      If the list is empty we will update DATA to the top of the list
      Otherwise, if there is already a node in the list, DATA is inserted at the top of the list and the top becomes NEXT
   */
int insert_to_list(List* list, void* data, int(*compre)(void*, void*));

/* @ Function: free_list
   @ Arguments: This function gets List* list , void(*free_data)(void*)-
   @ Description: The function frees the memory of all nodes in the list
   */
void free_list(List* list, void(*free_data)(void*));





#endif
