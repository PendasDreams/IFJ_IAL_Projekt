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
    char actual_char;
    state = START;


   while(1) {
        actual_char = getc(stdin);
        printf("%c", actual_char);
		token->line = line_count;


		switch(state){
			case START:{
                if ((actual_char == '_') || (actual_char >= 'a' && actual_char <= 'z') || (actual_char >= 'A' && actual_char <= 'Z') ) {
                    state = ID;
                    char *data = (char*) malloc(sizeof(char));
                    *data = actual_char;
                    Stack_Init(&s);
                    Stack_Push(&s, data);
                }
                else if (actual_char >= '0' && actual_char <= '9') {
                    state = NUMBER;
                    //initString(tokenStr);
                    //stringPush(tokenStr,actual_char);
                }
                else if (isspace(actual_char)) {
                    ;
                }
                else if (actual_char == EOF) {
                    token->type = EOFile;
                    return;
                }
                else {
                    switch (actual_char) {

                        case '<':
                            state = LESS;
                            break;

                        default:
                            print_errors(ERROR_LEX);
                            return;
                    }
                }
                break;
            }

            // case ID: {
            //     if ((actual_char == '_') || (actual_char >= 'a' && actual_char <= 'z') || (actual_char >= 'A' && actual_char <= 'Z') || (actual_char >= '0' && actual_char <= '9')) {
            //         Stack_Push(tokenStr, actual_char);
            //         state = ID;
            //     }
            //     else {
            //         lastChar = actual_char;
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



            case LESS:{                
                if (actual_char == '=') {
                    token->type = EQ_LESS;
                    return;
                }
                else if (actual_char == '?') {
                    state = HEADER;
                }
                else {
                    //lastChar = actual_char;
                    //charStreamSwitch = 0;
                    token->type = LESS;
                    return;
                }
                break;
            }

            case HEADER: {
                if (actual_char == 'p') {
                    state = HEADER1;
                }
                else {
                    print_errors(ERROR_LEX);
                    return;
                }
                break;
            }
            case HEADER1: {
                if (actual_char == 'h') {
                    state = HEADER2;
                }
                else {
                    print_errors(ERROR_LEX);
                    return;
                }
                break;
            }
            case HEADER2: {
                if (actual_char == 'p') {
                    state = HEADER3;
                }
                else {
                    print_errors(ERROR_LEX);
                    return;
                }
                break;
            }
            case HEADER3: {
                if (isspace(actual_char)) {
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