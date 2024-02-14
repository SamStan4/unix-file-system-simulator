#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINKED_LIST_DATA_SIZE 50

typedef struct linked_list_node
{
    char data[50];
    struct linked_list_node* link;

} linked_list_node;

typedef struct linked_list
{
    linked_list_node* head_ptr;
    linked_list_node* tail_ptr;

} linked_list;

//***************************************************************************************************************************************************************//
//     function call        : linked_list_constructor()
//     function description : this function initializes the linked list 
//     function inputs      : linked_list*
//     function outputs     : void
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

void linked_list_constructor(linked_list* list)
{
    list->head_ptr = NULL;
    list->tail_ptr = NULL;
}

//***************************************************************************************************************************************************************//
//     function call        : linked_list_destructor()
//     function description : this function initializes the linked list 
//     function inputs      : linked_list*
//     function outputs     : void
//     preconditions        : this function is only called on fresh linked lists
//     postconditions       : none
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

void linked_list_destructor(linked_list* list)
{
    linked_list_node* current_ptr = list->head_ptr, * next_ptr = list->head_ptr;

    while (current_ptr != NULL)
    {
        next_ptr = next_ptr->link;

        free(current_ptr);

        current_ptr = next_ptr;
    }

    list->head_ptr = NULL;
    list->tail_ptr = NULL;
}

//***************************************************************************************************************************************************************//
//     function call        : make_linked_list_node()
//     function description : this function creats a linked list node
//     function inputs      : void
//     function outputs     : linked_list_node*
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

linked_list_node* make_liked_list_node(void)
{
    linked_list_node* new_node = (linked_list_node*)malloc(sizeof(linked_list_node));
    int i = 0;

    if (new_node != NULL) // if so, initialize the data
    {
        new_node->link = NULL;

        for (i = 0; i < LINKED_LIST_DATA_SIZE; ++i) // make this all null characters
        {
            new_node->data[i] = '\0';
        }
    }

    return new_node;
}

//***************************************************************************************************************************************************************//
//     function call        : insert_at_front()
//     function description : this function inserts a node at the begining of a linked list
//     function inputs      : linked_list*, char*
//     function outputs     : void
//     preconditions        : the linked list has been initialized
//     postconditions       : none
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

void insert_at_front(linked_list* list, char* data)
{
    linked_list_node* new_node = make_liked_list_node();

    if (new_node != NULL)
    {
        strcpy(new_node->data, data);

        new_node->link = list->head_ptr;
        list->head_ptr = new_node;

        if ((list->head_ptr == NULL) && (list->tail_ptr == NULL)) // inserting on an empty list
        {
            list->tail_ptr = new_node;
        }
    }
}

//***************************************************************************************************************************************************************//
//     function call        : insert_at_back()
//     function description : this function inserts a node at the begining of a linked list
//     function inputs      : linked_list*, char*
//     function outputs     : void
//     preconditions        : the linked list has been initialized
//     postconditions       : none
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

void insert_at_back(linked_list* list, char* data)
{
    linked_list_node* new_node = make_liked_list_node();

    if (new_node != NULL)
    {
        strcpy(new_node->data, data);

        if ((list->head_ptr == NULL) && (list->tail_ptr == NULL)) // inserting on an empty list
        {
            list->tail_ptr = new_node;
            list->head_ptr = new_node;
        }
        else
        {
            list->tail_ptr->link = new_node;
            list->tail_ptr = new_node;
        }
    }
}





#endif