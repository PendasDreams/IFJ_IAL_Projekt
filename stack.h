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
	int data;

}ptrItem;

// type 1 = char
// type 2 = int


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







/**
 * @brief Inicializace zasobniku, top index == -1
 * @param stack Ukazatel na zásobník
 */
void Stack_Init( Stack* stack);

/**
 * @brief Kontrola zda je zásobník prázdný, 
 *
 * @param stack Ukazatel na zásobník
 *
 * @returns prázdný = 1, neprázdný 0
 */
bool Stack_IsEmpty( Stack* stack);

/**
 * @brief Kontrola zda je zásobnik plný
 *
 * @param stack Ukazatel na zásobník
 *
 * @returns Nenulovou hodnotu v případě, že je zásobník plný, jinak nulu
 */
int Stack_IsFull( Stack* stack );

/**
 * @brief Funkce vraci typ na vrcholu zásobniku
 *
 * @param stack Ukazatel na zásobník
 */
int Stack_Top_Type( Stack* stack);

/**
 * @brief Funkce vraci data na vrcholu
 *
 * @param stack Ukazatel na zásobník
 */
char* Stack_Top_Data( Stack* stack);

/**
 * @brief Odstraní vrcholový prvek zásobníku
 *
 * @param stack Ukazatel na zásobník
 */
void Stack_Pop( Stack* stack );

/**
 * @brief Vlozeni noveho prvku na vrchol zasobniku
 * @param stack Ukazatel na zásobník
 * @param value int k vložení
 * @param data data ulozena v tokenu
 */
void Stack_Push( Stack* stack,int data);

/**
 * @brief Zniceni zasobniku, free pameti
 * @param stack Ukazatel na zásobník
 */
void Stack_Destroy( Stack* stack);

/**
 * @brief Vraci ptrItem na vrchol zasobniku
 * @param stack Ukazatel na zásobník
 */
ptrItem* Stack_Top_Ptr( Stack* stack);

/**
 * @brief Pomocna funkce na vytiksnuti obsahu zasobniku
 * @param stack Ukazatel na zásobník
 */
void Stack_Print( Stack* stack);

/**
 * @brief Pomocna funkce ktera vlozi znak pred NON_TERM na zasobniku
 * @param stack Ukazatel na zásobník
 * @param type int k vložení
 */
int Stack_InsertBeforeNonTerm(Stack *stack, int type,char* data);

void stack_init_string (tStack_string* s);
bool stack_empty_string (tStack_string* s);
char * stack_top_string (tStack_string* s);
void stack_pop_string (tStack_string* s);
void stack_push_string (tStack_string* s, char * data);

#endif
