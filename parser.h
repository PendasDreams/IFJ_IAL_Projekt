#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "scanner.h"
// //#include "symtable.h"
// //#include "expression.h"



/**
 * @brief Makro ulozi do promenne token novy token, pri chybe vrati chybovy kod lexykalni analyzy
 * @param token Ukazatel na token
 */

void prog();
void prolog();
void eol_opt();
void st_list();  
void statement(); 
void func(); 
void assignment();
void stmtList();
void stmtListBracketed();
void fun_or_expr_2();

                                                  