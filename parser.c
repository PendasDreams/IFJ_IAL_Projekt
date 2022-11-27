#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "scanner.h"
#include "parser.h"











token_t next_token; //globalni token

void process(){

    for(int i = 0; i<= 10; i++){
    printf("\n==========================================\n");

    next_token = get_token(stdin);

        printf("\nPARSER=>\n token type is %d:\n", next_token.type);   
    }


} 