/**
 * @file error.c
 * @author xnovos14
 * @author 
 * @brief Error codes
 */


#include <stdio.h>


/**
 * @brief Chybové kódy překladače
 */


typedef enum error_code {
    ERROR_OK = 0, // není chyba
    ERROR_LEX = 1,
    ERROR_SYNTAX = 2,
    ERROR_SEM_DEF = 3,
    ERROR_SEM_PARAM = 4,
    ERROR_SEM_UNDEF_VAR = 5,
    ERROR_SEM_FUNC_RET = 6,
    ERROR_SEM_TYPE = 7,
    ERROR_SEM_OTHER = 8,
    ERROR_PROGRAM = 99, 
} error_code;

