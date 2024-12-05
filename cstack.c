#include "cstack.h"
#include <stddef.h>
#include <stdlib.h> 
#include <memory.h>

#define UNUSED(VAR) (void)(VAR)

struct node
{
    struct node* previous;
    unsigned int size;
    void* data;
};

struct stack
{
    int index;
    unsigned int size;
    struct node* node;
};

struct stackEntryes
{
    int size;
    struct stack* stackTable;
};
struct stackEntryes table = { 0 , NULL };

hstack_t stack_new(void)
{
    table.stackTable = realloc(table.stackTable, sizeof(struct stack) * (table.size + 1));// выдел€ем пам€ть дополниельную €чейку под новый стэк
    if (table.stackTable == NULL) //проверка на успешность выделени€ пам€ти
        return -1;
    if (table.size == 0) //если первый раз создаем стэк его индекс равен 0, если не первый то индекс равен наибольшему существующему стеку + 1
        table.stackTable[table.size].index = table.size;
    else
        table.stackTable[table.size].index = table.stackTable[table.size - 1].index + 1;
    table.stackTable[table.size].size = 0;
    table.size++;
    return table.stackTable[table.size - 1].index;
}

void stack_free(const hstack_t hstack)
{
    for (int i = 0; i < table.size; i++)
    {
        if (table.stackTable[i].index == hstack)
        {
            while (table.stackTable[i].size > 0) //удал€ем все элементы стека
            {
                char* dataOut = malloc(table.stackTable[i].node->size);
                stack_pop(hstack, dataOut, table.stackTable[i].node->size);
                free(dataOut);
            }
            if (i != (table.size - 1)) //если стек существует и элемент не последний сдвигаем масиив
            {
                while (i < (table.size - 1))
                {
                    table.stackTable[i].index = table.stackTable[i + 1].index;
                    table.stackTable[i].node = table.stackTable[i + 1].node;
                    table.stackTable[i].size = table.stackTable[i + 1].size;
                    i++;
                }
            }
            table.size--;
            struct stack* newTable = malloc(sizeof(struct stack) * (table.size));  //создаем буфер и копируем в него старый массив без последнего элемента
            memcpy(newTable, table.stackTable, sizeof(struct stack) * (table.size));
            free(table.stackTable);
            table.stackTable = newTable;
            break;
        }
    }
}

int stack_valid_handler(const hstack_t hstack)
{
    for (int i = 0; i < table.size; i++)
    {
        if (table.stackTable[i].index == hstack)
            return 0;
    }
    return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    for (int i = 0; i < table.size; i++)
    {
        if (table.stackTable[i].index == hstack)
            return table.stackTable[i].size;
    }
    return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if ((data_in != NULL) && (size != 0))
        for (int i = 0; i < table.size; i++)
        {
            if (table.stackTable[i].index == hstack)
            {
                table.stackTable[i].size++; //находим нужный стэк и создаем новый элемент, стек указывает  на верхний элемент, элемнт указывает на следующий
                struct node* newNode = malloc(sizeof(struct node));
                if (newNode == NULL)
                {
                    free(newNode);
                    return;
                }
                newNode->data = malloc(size);
                if (newNode->data == NULL)
                {
                    free(newNode->data);
                    free(newNode);
                    return;
                }
                memcpy(newNode->data, data_in, size);
                newNode->size = size;
                newNode->previous = table.stackTable[i].node;
                table.stackTable[i].node = newNode;
                break;
            }
        }
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    for (int i = 0; i < table.size; i++)
    {
        if (table.stackTable[i].index == hstack)
        {
            if ((table.stackTable[i].size > 0) && (table.stackTable[i].node->size <= size) && (data_out != NULL)) //если данные существуют
            {
                table.stackTable[i].size--;
                int nodeSize = table.stackTable[i].node->size;
                memcpy(data_out, table.stackTable[i].node->data, nodeSize); //передаем данные
                struct  node* oldNode = NULL;
                oldNode = table.stackTable[i].node;
                table.stackTable[i].node = (struct node*)table.stackTable[i].node->previous; //удал€ем элемент
                free(oldNode->data);
                free(oldNode);
                return nodeSize;
            }
        }
    }
    return 0;
}

