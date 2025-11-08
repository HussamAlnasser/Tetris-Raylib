#ifndef ARRAYLIST_H
#define ARRAYLIST_H
#include <stdio.h>
#include <stdlib.h>
#define ARRAY_LIST_STEP 2

typedef struct _array_list{
    int size;
    int actualSize;
    int *content;
} array_list;

void array_list_create(array_list *arrayList)
{
    arrayList->actualSize = arrayList->size = 0;
    arrayList->content = NULL;
}

int array_list_append(array_list *arrayList, int item)
{

    if(!arrayList->content)
    {
        arrayList->content = (int*)malloc(sizeof(int));
    }
    else
    {
        if(arrayList->size >= arrayList->actualSize)
        {
            if(arrayList->actualSize == 0)
            {
                arrayList->actualSize++;
            }
            int size = sizeof(int);
            int *tmp = (int*)realloc(arrayList->content,arrayList->actualSize*ARRAY_LIST_STEP*size);

            if(!tmp)
            {
                return -1;
            }

            arrayList->content = tmp;
            arrayList->actualSize *= ARRAY_LIST_STEP;
        }
    }

    arrayList->content[arrayList->size] = item;
    arrayList->size++;

    return 0;
}

void array_list_clear(array_list *arrayList)
{
    if(arrayList->content != NULL)
    {
        free(arrayList->content);
        arrayList->content = NULL;
    }
    arrayList->size = 0;
    arrayList->actualSize = 0;
}

#endif