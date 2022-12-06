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



// inicializace
void Stack_Init(Stack *stack)
{
	if (stack == NULL){
        printErrorIn(ERROR_PROGRAM);

	}
	
	else{

		stack->top = NULL; 
	}
	
}

//pokud je stack prázdný, vrací true
bool Stack_IsEmpty(Stack* stack)
{
	if (stack->top == NULL)
	{
		return true;
	}
	return false; 
}



// vrati vrchol zasobniku
int Stack_Top_Type(Stack *stack)
{

	if (Stack_IsEmpty(stack))
	{
		exit(99);
	}

	return stack->top->type; 
}



//odstraní položku z vrcholu
void Stack_Pop(Stack *stack)
{

	if (Stack_IsEmpty(stack))  
	{
		printErrorIn(ERROR_PROGRAM);
	}
	else if(!Stack_IsEmpty(stack)){

		ptrItem *item = stack->top;
		stack->top = stack->top->next;
		free(item);
	}

}

//odstraní položku na vrchol
void Stack_Push(Stack *stack, char data)
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

// smaže celý stack
void Stack_Destroy(Stack *stack)
{

	while (!Stack_IsEmpty(stack))
	{
		Stack_Pop(stack);
	}

}

// vytiskne stack
void Stack_Print_C(Stack* stack){

	ptrItem* tmp = stack->top;
	
	while(tmp!=NULL)
	{
		printf("%i %c \n",tmp->type,tmp->data);
		tmp = tmp->next;
	}



}
