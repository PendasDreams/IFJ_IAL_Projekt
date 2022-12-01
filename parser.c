#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "scanner.h"
#include "parser.h"











token_t next_token; //globalni token
bool in_function = false, in_block = false;



int condition(int skip)
{
    if (next_token.type != TK_PAR_LEFT) {
        printErrorIn(ERR_PARSE);
        return 0;
    }

    next_token = get_token(stdin);

    // Use dedicated place for result of condition
    //int exprRes = generalExpr(skip, stackFrameReserved - 1, NULL);
    //if (printErrorIn(ERR_PARSE))
    // return 0;

    // Right bracket loaded by expr
    if (next_token.type != TK_PAR_RIGHT) {
        printErrorIn(ERR_PARSE);
        return 0;
    }

    next_token = get_token(stdin);
    return 0;
}




void nparamList(int defarg)
{
    switch (next_token.type) {
        case TK_PAR_RIGHT:
            // Rule 21
            break;

        case TK_COMMA:
            // Rule 22
            next_token = get_token(stdin);

            if (next_token.type != TK_VARIABLE) {
                printErrorIn(ERR_PARSE);
                return;
            }

            // if (getError())
            //     return;

            

            next_token = get_token(stdin);
            // Test for default argument
            if (next_token.type == TK_EQ) {
                for(int i=0; i<2;i++){
                next_token = get_token(stdin);
                } // skip also default value handled in addParameter
                defarg = 1;
            }
            else if (defarg) {
                printErrorIn(ERR_PARSE);
                return;
            }

            nparamList(defarg);
            // if (getError())
            //     return;

            break;

        default:
            printErrorIn(ERR_PARSE);
    }
}




void paramList(){
    switch (next_token.type) {
        case TK_PAR_RIGHT:
            // Rule 19
            break;

        case TK_VARIABLE: {
            // Rule 20
            int defarg = 0;

            next_token = get_token(stdin);
            // Test for default argument
            if (next_token.type == TK_EQ) {
                for(int i=0; i<2;i++){
                next_token = get_token(stdin);
                }   // skip also default value handled in addParameter
                defarg = 1;
            }

            nparamList(defarg);
            // if (getError())
            //     return;


            break;
        }
        default:
            printErrorIn(ERR_PARSE);
    }
}


void function()
{
    // Callable only from Rule 3 with terminal. Function
    // doesn't need switch.
    next_token = get_token(stdin);

    if (next_token.type != TK_ID) {
        printErrorIn(ERR_PARSE);
        return;
    }

    next_token = get_token(stdin);

    if (next_token.type != TK_PAR_LEFT) {
        printErrorIn(ERR_PARSE);
        return;
    }

    next_token = get_token(stdin);

    paramList();
    // if (getError())
    //     return;

    // Right bracket loaded by paramList.
    if (next_token.type != TK_PAR_RIGHT) {
        printErrorIn(ERR_PARSE);

        return;
    }

    next_token = get_token(stdin);


    stmtListBracketed();
    // if (getError())
    //     return;

}

void stmtList()
{
    switch (next_token.type) {
        case TK_RIGHT_CURLY_BRACKET:
            // Rule 6
            break;

        case TK_VARIABLE:
            // Rule 7
            statement();
            // if (getError())
            //     return;

            stmtList();
            // if (getError())
            //     return;

            break;

        case TK_KWRD:
            switch (next_token.value.keyword_value) {
                case KEY_RETURN:
                case KEY_IF:
                case KEY_WHILE:
                    // Rule 7
                    statement();
                    //if (getError())
                    //    return;

                    stmtList();
                    //if (getError())
                    //    return;

                    break;

                default:
            printErrorIn(ERR_PARSE);
            }
            break;

        default:
        printErrorIn(ERR_PARSE);
    }
}




void stmtListBracketed()
{
    if (next_token.type != TK_LEFT_CURLY_BRACKET) {
        printErrorIn(ERR_PARSE);
        return;
    }

    next_token = get_token(stdin);

    stmtList();
    //if (getError())
    //    return;

    // Right curly bracket loaded by stmtList
    if (next_token.type != TK_RIGHT_CURLY_BRACKET) {
        printErrorIn(ERR_PARSE);
        return;
    }

    next_token = get_token(stdin);
}



void prolog(){
        // rule 1
    printf("\n=====we are in prolog now=====\n");

    for(int i = 0; i <= 1; i++){
    next_token = get_token(stdin);

        printf("\n\nNEXT TOKEN ID:%d\n\n", next_token.type);
        if(next_token.type == TK_EOF){
            printErrorIn(ERR_PARSE);
            printf("finiito of the soubor");
            return;
        }
        if(next_token.type == TK_PHP){
            printf("\n===rule one was OK => HEADER DONE===\n");
        }
        else{
            printErrorIn(ERR_PARSE);
            printf("\n===rule FAILED => HEADER DONE===\n");

        }
    }

    prog();
}



void prog(){
    printf("\n=====we are in PROGRAM now=====\n");

    next_token = get_token(stdin);

    switch (next_token.type)
    {
    case TK_EOF:
     // Rule 4
        // if (secondRun) {
        //     generateInstruction(IST_Nullify, ISM_NoConst, 0, -1, 0);
        //     generateInstruction(IST_Return, ISM_NoConst, 0, 0, 0);
        // }
    break;

    case TK_VARIABLE:
            // Rule 2
            statement();

            //if (getError())
            //    return;

            prog();
            //if (getError())
            //    return;

            break;

    case TK_KWRD:
        switch (next_token.value.keyword_value) {
            case KEY_FUNCTION:
                // Rule 3
                function();
                // if (getError())
                //     return;

                prog();
                //if (getError())
                //    return;

                break;

            case KEY_RETURN:
            case KEY_IF:
            case KEY_WHILE:
                // Rule 2
                statement();
                // if (getError())
                //     return;

                prog();
                // if (getError())
                //     return;

                break;

            default:
                printErrorIn(ERR_PARSE);
            }
            break;
     case TK_ID:
            // Rule 2
            statement();
            //if (getError())
            //    return;

            prog();
            //if (getError())
            //    return;

            break;

    default:
        printErrorIn(ERR_PARSE);
    }

    // int i = 0;
    // while(next_token.type != TK_EOF){

    // //while(i <= 80){
    
    // printf("\n %d ==========================================\n", i);

    // next_token = get_token(stdin);

    //     printf("\nPARSER=>\n token type is %d:\n", next_token.type);   
    //     i++;
    // }
} 

void fun_or_expr() {
    printf("===In fun_or_expr===\n");
    //pravidlo 22*
    if (next_token.type == TK_STRING || next_token.type == TK_FLOAT
        || next_token.type == TK_INT || (next_token.type == TK_KWRD && next_token.value.keyword_value == 0)
        || next_token.type == TK_PAR_LEFT) {
        //first = next_token;
        /*****PSA*******/
        //next_token = expressionParse(stdin, &first, NULL, 1);

        //print_pop = 1;
        }
    //pravidlo 20
    else if (next_token.type == TK_ID){
        //first = next_token;
        next_token = get_token(stdin);
        fun_or_expr_2();
    }
    else
        printErrorIn(ERR_PARSE);
}

void fun_or_expr_2() {
    printf("===In fun_or_expr2===\n");
    //pravidlo 23*
    if (next_token.type == TK_ADD || next_token.type == TK_SUB ||
        next_token.type == TK_MUL || next_token.type == TK_DIV ||
        next_token.type == TK_LESS ||
        next_token.type == TK_EQ_LESS || next_token.type == TK_GREATER ||
        next_token.type == TK_COMPARSION || next_token.type == TK_NEG_COMPARSION ||
        next_token.type == TK_EQ) {
            //second = next_token;
            /*****PSA*******/
            //next_token = expressionParse(stdin, &first, &second, 2);

            //print_pop = 1;
    }
    //pravidlo 21
    else if (next_token.type == TK_PAR_LEFT) {
        next_token = get_token(stdin);

        //if (strcmp(next_token.value.string, "print"))
//            print_instruction("\nCREATEFRAME\n");

        //arg_num = 0;
    //arg_list();
        //stack_sem_push(&stack_semantic, FUN_CALL, first.value.string);
        //arg_num = 0;

        //if (strcmp(next_token.value.string, "print")){}
    //        print_instruction("CALL %s\n", first.value.string);

        //print_assign_fun = 1;

        if (next_token.type == TK_PAR_RIGHT)
            next_token = get_token(stdin);
        else
            printErrorIn(ERR_PARSE);
    }
    //pravidlo 30
    else if (next_token.type == TK_EOL) {
        //stack_sem_push(&stack_semantic, VAR_USE, first.value.string); //pouze kontrola samotneho identifikatoru

        //print_assign_one_var = 1;
    }
    else
        printErrorIn(ERR_PARSE);
}

void expr_or_assign()
{
    printf("===we are in expr_or_assign now ===\n");

    if (next_token.type == TK_ADD || next_token.type == TK_SUB ||
        next_token.type == TK_DIV || next_token.type == TK_MUL || 
        next_token.type == TK_LESS ||
        next_token.type == TK_EQ_LESS || next_token.type == TK_GREATER ||
        next_token.type == TK_EQ_GREATER || next_token.type == TK_NEG_COMPARSION ||
        next_token.type == TK_COMPARSION) {
        //second = next_token;
        /*****PSA*******/
        //next_token = expressionParse(stdin, &first, &second, 2);
    }
    //pravidlo 24
    else if (next_token.type == TK_PAR_LEFT){
        next_token = get_token(stdin);



        if (next_token.type == TK_PAR_RIGHT) {
            next_token = get_token(stdin);
}         printErrorIn(ERR_PARSE);

    }

    //pravidlo 6
    else if (next_token.type == TK_EQ) {
        next_token = get_token(stdin);

        fun_or_expr();
    }
    else
        printErrorIn(ERR_PARSE);



    // if (next_token.type == TOKEN_MATH_PLUS || next_token.type == TOKEN_MATH_MINUS ||
    //     next_token.type == TOKEN_MATH_DIV || next_token.type == TOKEN_MATH_MUL ||
    //     next_token.type == TOKEN_MATH_INT_DIV || next_token.type == TOKEN_LESS ||
    //     next_token.type == TOKEN_LESS_EQ || next_token.type == TOKEN_GREATER ||
    //     next_token.type == TOKEN_GREATER_EQ || next_token.type == TOKEN_NOT_EQ ||
    //     next_token.type == TOKEN_EQ) {
    //     second = next_token;
    //     /*****PSA*******/
        //next_token = expressionParse(stdin, &first, &second, 2);
    //}

}

  


void statement()
{
printf("===we are in statement now===\n");

    switch (next_token.type) {
        case TK_VARIABLE:
            // Rule 8

            next_token = get_token(stdin);

            expr_or_assign();
            //if (getError())
            //    return;

            // Semicolon loaded by assignment
            printf("===semi colon testing====\n");
            if (next_token.type != TK_SEMI_COLON) {
                printErrorIn(ERR_PARSE);
                return;
            }
            break;
            

        case TK_KWRD:
            switch (next_token.type) {
                case KEY_RETURN:
                    // Rule 9
                    next_token = get_token(stdin);;

                    // Semicolon loaded by expr
                    if (next_token.type != TK_SEMI_COLON) {
                        printErrorIn(ERR_PARSE);
                        return;
                    }

                    next_token = get_token(stdin);

                    break;


                case KEY_IF: {
                    // Rule 12
                    next_token = get_token(stdin);

                    condition(0);

                    stmtListBracketed();
                    // if (getError())
                    //     return;

                    break;
                }

                case KEY_WHILE: {
                    // Rule 13 (almost same as Rule 12, missing just elseif call)
                    next_token = get_token(stdin);

                    stmtListBracketed();
                    // if (getError())
                    //     return;

                        
                    break;
                }

            default:
                printErrorIn(ERR_PARSE);
        }       

        case TK_ID:
            // Rule 8

            next_token = get_token(stdin);

            expr_or_assign();
            //if (getError())
            //    return;

            // Semicolon loaded by assignment
            if (next_token.type != TK_SEMI_COLON) {
                printErrorIn(ERR_PARSE);
                return;
            }
            break;

          default:
                printErrorIn(ERR_PARSE);
            
    }
}
