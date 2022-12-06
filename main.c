/**
 * @file scanner.c
 * @author Denis Novosád xnovos14
 * @brief Main
 */

#include <stdio.h>
#include "scanner.h"



int main(int argc, char const *argv[])
{

	if(argc == 1){

		token_t next_token;


		next_token = get_token(stdin);


		while(next_token.type != TK_EOF){
			if(next_token.type == TK_ERROR){
				return 0;
			}
			printf("\n\n===typ tokenu je %d===\n\n", next_token.type);
			next_token = get_token(stdin);
		} 	
		return 0;
	}
	
	return 0;
	
}
