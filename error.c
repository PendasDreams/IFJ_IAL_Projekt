#include <stdio.h>
#include "parser.h"
#include "scanner.h"
#include "error.h"
//#include "expression.h"
//#include "symtable.h"


void printError(error_code err, TOKEN *token){
    switch(err){
        case ERROR_LEX:
            fprintf(stderr, "Fault at line %d: LEX FAULT\n", token->line);
            return err;
        case ERROR_SYNTAX:
            fprintf(stderr, "Fault at line %d: SYNTAX FALUT\n", token->line);
            return err;
        case ERROR_SEM_DEF:
            fprintf(stderr, "Fault at line %d: SEM-FAULT-DEF\n", token->line);
            return err;
        case ERROR_SEM_PARAM:
            fprintf(stderr, "Fault at line %d: ERROR_SEM_PARAM\n", token->line);
            return err;
        case ERROR_SEM_UNDEF_VAR:
            fprintf(stderr, "Fault at line %d: ERROR_SEM_UNDEF_VAR\n", token->line);
            return err;
        case ERROR_SEM_FUNC_RET:
            fprintf(stderr, "Fault at line %d: ERROR_SEM_FUNC_RET\n", token->line);
            return err;
        case ERROR_SEM_TYPE:
            fprintf(stderr, "Fault at line %d: ERROR_SEM_TYPE\n", token->line);
            return err;
        case ERROR_SEM_OTHER:
            fprintf(stderr, "Fault at line %d: ERROR_SEM_OTHER\n", token->line);
            return err;
        default:
            return err;
    }
}