#ifndef ARRAY_LIST_H_
#define ARRAY_LIST_H_
#include <stdint.h>
#include "gfx.h"

typedef struct
{
    array_list_item *next = 0;
    array_list_item *prev = 0;
    void *item;
    uint32_t id;
} array_list_item;

typedef struct
{
    void *start = 0;
    void *end = 0;
    uint32_t size;
} array_list_head;

array_list_head *createArrayList()
{
    array_list_head *arrayList = (array_list_head *)gfxAlloc(sizeof(array_list_head));
    arraylist->start = 0;
    arraylist->end = 0;
    arraylist->size = 0;
    return arrayList;
}

void appendItem(uint32_t id, void *item, array_list_head *head)
{
    array_list_item *arrayItem = (array_list_item *)gfxAlloc(sizeof(array_list_item));
    arrayItem->id = id;
    if (head->start == 0 && head->end == 0 && head->size == 0)
    {
        head->start = arrayItem;
        head->end = arrayItem;
        head->size = 1;
    }
    else if (head->start == head->end && head->size == 1)
    {
        head->end = arrayItem;
        head->size += 1;
    }
    else if (head->size > 1)
    {
        array_list_item *temp = head->end;
        head->end = arrayItem;

        temp->next = arrayItem;
        arrayItem->prev = temp;
        head->size += 1;
    }
}

array_list_item *findArrayItem(uint32_t id, array_list_head *head)
{
    array_list_item* item = head->start;
    for (uint32_t i = 0; i < head->size; i++)
    {
        if(item == 0) {
            return 0;
        }
        if(item->id == id) {
            return item;
        }
        if(item == head->end){
            return 0;
        }
        item = item->next;
    }
    return 0;
}

void removeItem(uint32_t id, array_list_head *head, freeItem = TRUE)
{
    if (head->start == 0 && head->end == 0 && head->size == 0)
    {
        return;
    }
    array_list_item* item = findArrayItem(id, head);
    if(!item){
        return;
    }
    if (head->start == head->end && head->size == 1)
    {
        freeArrayItem(head->start, freeItem);
        head->end = 0;
        head->start = 0;
        head->size = 0;
    }
    else if (head->size == 2)
    {
        if(isFirstItem(item,head->start)) {
            freeArrayItem(head->start, freeItem);
            head->start = head->end;
            head->end->prev = 0;
        } else if(isLastItem(item, head->end)) {
            head->end = head->start;
            head->start->next = 0;
            freeArrayItem(head->end, freeItem);
        }
        head->size = 1;
    } else if(head->size > 2) {
        if(isFirstItem(item,head->start)) {    
            array_list_item* temp = head->start;   
            head->start = temp->next;
            

            head->end->prev = 0;
        }
        freeArrayItem(head->start, freeItem);
        }
    }
}

void isFirstItem(array_list_item* arrayItem, array_list_head* head) {
    return arrayItem == head->start;
}

void isLastItem(array_list_item* arrayItem, array_list_head* head) {
    return arrayItem == head->end;
}

void freeArrayItem(array_list_item* arrayItem, freeItem = TRUE) {
    if(freeItem) {
        gfxFree(arrayItem->item);
    }
    gfxFree(arrayItem);
}

#endif