/**
 * @file scanner.c
 * @author
 * @brief Lexikalni analyzator
 */

#include <stdio.h>
#include <ctype.h>

#include <stdlib.h>
#include "error.h"
#include "scanner.h"
#include "stack.h"


 
int state;
int line_count = 1;
Stack s;

/*
* Funkcia nevracia token, iba spracuvava znazky.
* 3 moznosti co tato funkcia robi
* 1. Dostane znak, ulozi ho do bufferu a bud sa posunie na dalsi stav alebo zostane v tom istom. (zalezi na stave)
* 2. Dostane znak, je v konecnom stave automatu a znak uz nevie spracovat tak vyhlasi token za kompletny.
* 3. Dostane znak, nieje v konecnom stave a znak nevie spracovat, tak vyhlasi lexikalnu chybu.
*/
void SpracujZnak(TOKEN *token) 
{
    char actual_char[2] = "\0"; /* gives {\0, \0} */
    state = START;




   while(1) {
        actual_char[0] = fgetc(stdin);
        printf("%c", actual_char[0]);
		token->line = line_count;


            char* p = actual_char;
            Stack_Init(&s);
            Stack_Push(&s,1,p);
            Stack_Print(&s);


		switch(state){
			case START:{
                printf("CASE START=======\n");
                if ((actual_char[0] == '_') || (actual_char[0] >= 'a' && actual_char[0] <= 'z') || (actual_char[0] >= 'A' && actual_char[0] <= 'Z') ) {
                    state = ID;
                    char* p = "d";
                    Stack_Init(&s);
                    Stack_Push(&s,1,p);
                    Stack_Print(&s);
                }
                else if (actual_char[0] >= '0' && actual_char[0] <= '9') {
                    state = NUMBER;
                    //initString(tokenStr);
                    //stringPush(tokenStr,actual_char[0]);
                }
                else if (isspace(actual_char[0])) {
                    ;
                }
                else if (actual_char[0] == EOF) {
                    token->type = EOFile;
                    return;
                }
                else {
                    switch (actual_char[0]) {

                        case '<':
                            printf("CASE LESS=======\n");
                            state = LESS;
                            break;
                        default:
                            print_errors(ERROR_LEX);
                            return;
                    }
                }
                                            printf("CASE LESS=======\n");

                break;
            }


            // case ID: {
            //     if ((actual_char[0] == '_') || (actual_char[0] >= 'a' && actual_char[0] <= 'z') || (actual_char[0] >= 'A' && actual_char[0] <= 'Z') || (actual_char[0] >= '0' && actual_char[0] <= '9')) {
            //         Stack_Push(tokenStr, actual_char[0]);
            //         state = ID;
            //     }
            //     else {
            //         lastChar = actual_char[0];
            //         charStreamSwitch = 0;
            //         KeywordTokenType keywordType = strToKeyword(tokenStr); // FUNCTION CHECKS IF IDENTIFIER, WHICH HAS BEEN FOUND AINT A RESERVED ( KEYWORD ) WORD
            //         if (keywordType == KTT_None) {
            //             token->type = STT_Identifier;
            //             return;
            //         }
            //         else {
            //             deleteString(tokenStr);
            //             if (keywordType == KTT_True) {
            //                 token->type = STT_Bool;
            //                 token->n = 1;
            //             }
            //             else if (keywordType == KTT_False) {
            //                 token->type = STT_Bool;
            //                 token->n = 0;
            //             }
            //             else if (keywordType == KTT_Null) {
            //                 token->type = STT_Null;
            //             }
            //             else if (keywordType == KTT_And) {
            //                 token->type = STT_AndLow;
            //             }
            //             else if (keywordType == KTT_Or) {
            //                 token->type = STT_OrLow;
            //             }
            //             else {
            //                 token->type = STT_Keyword;
            //                 token->keywordType = keywordType;
            //             }
            //             return;
            //         }
            //     }
            //     break;
            // }


                printf("CASE LESS=======\n");
            case LESS:{         
                if (actual_char[0] == '=') {
                    token->type = EQ_LESS;
                    return;
                }
                else if (actual_char[0] == '?') {
                    state = HEADER;
                }
                else {
                    //lastChar = actual_char[0];
                    //charStreamSwitch = 0;
                    token->type = LESS;
                    return;
                }
                break;
            }

            case HEADER: {
                if (actual_char[0] == 'p') {
                    state = HEADER1;
                }
                else {
                    print_errors(ERROR_LEX);
                    return;
                }
                break;
            }
            case HEADER1: {
                if (actual_char[0] == 'h') {
                    state = HEADER2;
                }
                else {
                    print_errors(ERROR_LEX);
                    return;
                }
                break;
            }
            case HEADER2: {
                if (actual_char[0] == 'p') {
                    state = HEADER3;
                }
                else {
                    print_errors(ERROR_LEX);
                    return;
                }
                break;
            }
            case HEADER3: {
                if (isspace(actual_char[0])) {
                    token->type = TK_PHP;
                    return;
                }
                else {
                    print_errors(ERROR_LEX);
                    return;
                }
            }
        }
        return;
    }
    return;
}