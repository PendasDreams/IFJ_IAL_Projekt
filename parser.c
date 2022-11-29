#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "scanner.h"
#include "parser.h"











token_t next_token; //globalni token

void process(){

        int i = 0;
    while(next_token.type != TK_EOF){

    //while(i <= 100){
    
    printf("\n %d ==========================================\n", i);

    next_token = get_token(stdin);

        printf("\nPARSER=>\n token type is %d:\n", next_token.type);   
        i++;
    }


} 