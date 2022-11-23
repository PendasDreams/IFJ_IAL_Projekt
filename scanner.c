/**
 * @file scanner.c
 * @author
 * @brief Lexikalni analyzator
 */

#include <stdio.h>
#include <ctype.h>

#include "scanner.h"

 
int state;
int line_count = 1;

/*
* Funkcia nevracia token, iba spracuvava znazky.
* 3 moznosti co tato funkcia robi
* 1. Dostane znak, ulozi ho do bufferu a bud sa posunie na dalsi stav alebo zostane v tom istom. (zalezi na stave)
* 2. Dostane znak, je v konecnom stave automatu a znak uz nevie spracovat tak vyhlasi token za kompletny.
* 3. Dostane znak, nieje v konecnom stave a znak nevie spracovat, tak vyhlasi lexikalnu chybu.
*/
int SpracujZnak(TOKEN *token) 
{
    char actual_char;


   while(1) {
        actual_char = getc(stdin);
        printf("%c", actual_char);
		token->line = line_count;

		switch(state){
			case START:
				switch(actual_char){
					// TODO
                }
        }
    }
}