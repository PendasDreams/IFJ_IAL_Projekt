/**
 * @file main.c
 * @author xnovos14
 * @author 
 * @brief Main
 */

#include <stdio.h>
#include "scanner.h"



int main(int argc, char const *argv[])
{

	token_t next_token;


	next_token = get_token(stdin);


	while(next_token.type != TK_EOF){

	// for(int i =0; i<5; i++){ 

		if(next_token.type == TK_ERROR){
			return 0;
		}

		printf("\n\n===typ tokenu je %d===\n\n", next_token.type);
		next_token = get_token(stdin);
	} 
	
	return 0;
}
