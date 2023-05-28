

#include "list.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void start_list(List* list)
{
    list->head = NULL;
}


void* get_from_list(List* list, void* data, int(*compre)(void*, void*))
{
    Node* node = NULL;

    for (node = list->head; node != NULL; node = node->next)

    {
        if (compre(node->data, data) == 0)
            return node->data;
    }

    return NULL;
}

int insert_to_list(List* list, void* data, int(*compre)(void*, void*))
{
    Node* node, * temp;

    /*If already in the list*/
    if (get_from_list(list, data, compre) != NULL)
    {
        return 0;
    }

    node = (Node*)malloc(sizeof(Node));

    if (node == NULL)
    {
        ErrorMemory(memoryAllocate);
    }

    node->data = data;
    node->next = NULL;

    /*the list is empty*/
    if (list->head == NULL)
    {
        list->head = node;
    }

    /*puts node at the top of the list*/
    else
    {
        temp = list->head;
        list->head = node;
        node->next = temp;
    }

    return 1;
}



void free_list(List* list, void(*free_data)(void*))
{
    Node* node, * temp;

    for (node = list->head; node != NULL; node = temp)
    {
        if (free_data != NULL)
            free_data(node->data);

        temp = node->next;
        free(node);
    }
}

