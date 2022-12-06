/**
 * @file stack.h
 * @author xnovos14
 * @brief Deklarace pomocnych funkci pro zasobnik
*/

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#ifndef STACK_H
#define STACK_H

typedef struct item {

	struct item *next;
	int type;
	char data;

}ptrItem;

typedef struct 
{
	ptrItem *top;

} Stack;

typedef struct telem_string{
    char * data;
    struct telem_string* next;
} tElem_string;

typedef struct {
        tElem_string* top;
} tStack_string;


void Stack_Init( Stack* stack);
bool Stack_IsEmpty( Stack* stack);
int Stack_IsFull( Stack* stack );
void Stack_Pop( Stack* stack );
void Stack_Push( Stack* stack,char data);
void Stack_Destroy( Stack* stack);
void Stack_Print_C( Stack* stack);

#endif
