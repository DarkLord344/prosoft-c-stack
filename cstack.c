#include "cstack.h"
#include <stddef.h>
#include <stdlib.h> 
#include <memory.h>

#define UNUSED(VAR) (void)(VAR)

struct node
{
    const struct node* previous;
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
    table.stackTable = realloc(table.stackTable, sizeof(struct stack) * (table.size + 1));// �������� ������ ������������� ������ ��� ����� ����
    if (table.stackTable == NULL) //�������� �� ���������� ��������� ������
        return -1;
    if (table.size == 0) //���� ������ ��� ������� ���� ��� ������ ����� 0, ���� �� ������ �� ������ ����� ����������� ������������� ����� + 1
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
            if (i != (table.size - 1)) //���� ���� ���������� � ������� �� ��������� �������� ������
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
            struct stack* newTable = NULL;
            newTable = malloc(sizeof(struct stack) * (table.size));  //������� ����� � �������� � ���� ������ ������ ��� ���������� ��������
            for (int i = 0; i < table.size; i++)
            {
                newTable[i] = table.stackTable[i];
            }
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
                table.stackTable[i].size++; //������� ������ ���� � ������� ����� �������, ���� ���������  �� ������� �������, ������ ��������� �� ���������
                struct node* newNode = NULL;
                newNode = realloc(newNode, sizeof(struct node));
                newNode->data = (char*)data_in;
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
            if (table.stackTable[i].size > 0) //���� ������ ����������
            {
                table.stackTable[i].size--;
                unsigned int sizeNode;
                if (table.stackTable[i].node->size < size) //��������� ����� �� ������� ������
                    sizeNode = table.stackTable[i].node->size;
                else
                    sizeNode = size;
                memcpy(data_out, table.stackTable[i].node->data, sizeNode); //�������� ������
                struct  node* oldNode = NULL;
                oldNode = table.stackTable[i].node;
                table.stackTable[i].node = (struct node*)table.stackTable[i].node->previous; //������� �������
                free(oldNode);
                return sizeNode;
            }
        }
    }
    return 0;
}

