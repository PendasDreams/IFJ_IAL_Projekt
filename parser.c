#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "scanner.h"
#include "parser.h"











token_t next_token; //globalni token

void process(){

    next_token = get_token(stdin);


} 