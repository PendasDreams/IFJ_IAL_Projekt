/**
 * @file stack.c
 * @author xnovos14
 * @brief Implentace pomocnych funkci pro zasobnik
*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stack.h"
#include "error.h"
#include "scanner.h"




void Stack_Init(Stack *stack)
{
	if (stack == NULL)
        printErrorIn(ERROR_PROGRAM);
	stack->top = NULL; // nastaveni inicializace
}


bool Stack_IsEmpty(Stack* stack)
{
	if (stack->top == NULL)
	{
		return true;
	}
	
	return false; //pokud je index rovny inicializaci vratim true
}




int Stack_Top_Type(Stack *stack)
{

	if (Stack_IsEmpty(stack)) //pokud je prazdny nejsou v nem data
	{
		exit(99);
	}

	return stack->top->type; // vrati vrchol zasobniku
}




void Stack_Pop(Stack *stack)
{

	if (Stack_IsEmpty(stack)) //pokud je prazdny 
	{
		printErrorIn(ERROR_PROGRAM);
	}
	else if(!Stack_IsEmpty(stack)){

		ptrItem *item = stack->top;
		stack->top = stack->top->next;
		free(item);
	}

}


void Stack_Push(Stack *stack, int data)
{
	if (stack == NULL)
		printErrorIn(ERROR_PROGRAM);
	else{
		ptrItem *item = malloc(sizeof(ptrItem));
		if (item==NULL)
		{
			printErrorIn(ERROR_PROGRAM);
		}
		item->data= data;
		item->next=stack->top;
		stack->top=item;
	}	
	
	return;

}

void Stack_Destroy(Stack *stack)
{


	while (!Stack_IsEmpty(stack)) //pokud neni prazdny
	{
		ptrItem *item=NULL;
		item = stack->top;
		stack->top = item->next;
		free(item);
	}

	

}


void Stack_Print(Stack* stack){
	printf("\nsup\n");
	ptrItem* tmp = stack->top;
	printf("-------------STACK PRINT-------- ID types z expression.h \n");
	while(tmp!=NULL)
	{
		printf("%i %d \n",tmp->type,tmp->data);
		tmp = tmp->next;
	}
	printf("---konec printu-----\n");


}

//==================================== String stack


void stack_init_string(tStack_string* s) {
	if (s == NULL)
		printErrorIn(ERROR_PROGRAM);
	else
		s->top = NULL;
}

bool stack_empty_string(tStack_string* s) {
    if (s == NULL)
		return true;
    else
        return false;
}

char * stack_top_string(tStack_string* s) {
	if (stack_empty_string(s) || stack_empty_string(s))
		printErrorIn(ERROR_PROGRAM);
	else
		return s->top->data;

	return NULL;
}

void stack_pop_string(tStack_string* s) {
	if (s == NULL)
		printErrorIn(ERROR_PROGRAM);
	else if (!stack_empty_string(s)) {
            tElem_string* del = NULL;
			del = s->top;
            s->top = s->top->next;
			free(del);
        }
}

void stack_push_string(tStack_string* s, char * data) {
	if (s == NULL)
		printErrorIn(ERROR_PROGRAM);
	else {
        tElem_string* insert = (tElem_string*)malloc(sizeof(tElem_string));
        if (insert == NULL)
		printErrorIn(ERROR_PROGRAM);

        insert->data = data;
        insert->next = s->top;
        s->top = insert;
	}
}