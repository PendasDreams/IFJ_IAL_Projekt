/**
 * @file scanner.c
 * @author
 * @brief Lexikalni analyzator
 */


#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#include "scanner.h"
#include "stack.h"
#include <errno.h>
#include <stdint.h>



int printError(error_code_t err, token_t *token);




int state;
int line_count = 1;
int first_token = 1;



int printError(error_code_t err, token_t *token){
    switch(err){
        case ERROR_LEX:
            fprintf(stderr, "Faul at line %d: ERROR_LEX\n", token->line);
            return err;
        case ERROR_SYNTAX:
            fprintf(stderr, "Faul at line %d: ERROR_SYNTAX\n", token->line);
            return err;
        case ERROR_SEM_DEF:
            fprintf(stderr, "Faul at line %d: ERROR_SEM_DEF\n", token->line);
            return err;
        case ERROR_SEM_PARAM:
            fprintf(stderr, "Faul at line %d: ERROR_SEM_PARAM\n", token->line);
            return err;
        case ERROR_SEM_UNDEF_VAR:
            fprintf(stderr, "Faul at line %d: ERROR_SEM_UNDEF_VAR\n", token->line);
            return err;
        case ERROR_SEM_FUNC_RET:
            fprintf(stderr, "Faul at line %d: ERROR_SEM_FUNC_RET\n", token->line);
            return err;
        case ERROR_SEM_TYPE:
            fprintf(stderr, "Faul at line %d: ERROR_SEM_TYPE\n", token->line);
            return err;
        case ERROR_SEM_OTHER:
            fprintf(stderr, "Faul at line %d: ERROR_SEM_OTHER\n", token->line);
            return err;
        default:
            return err;
    }
}

int printErrorIn(error_code_t err){
        fprintf(stderr, " %d Inner programm error\n", err);
        return err;
}


char two_digit_hex_to_dec (char first, char second) {
    char dec = 0;

    if (isdigit(first))
        dec = 16 * (first - '0');
    else if (first >= 'A' && first <= 'F')
        dec = 16 * (first - 'A' + 10);
    else
        dec = 16 * (first - 'a' + 10);

    if (isdigit(second))
        dec += second - '0';
    else if (second >= 'A' && second <= 'F')
        dec += second - 'A' + 10;
    else
        dec += second - 'a' + 10;
    return dec;
}



int str_to_int(char* string_number) {
    long int int_number = 0;

    while (*string_number) {
        if (int_number > (int)((unsigned)(-1)/2)) {
            return 1;
        }
        int_number *= 10;
        int_number += (string_number[0]-48);
        string_number++;
    }

    return int_number;
}

char* convert_to_str(char* input) {
    size_t shift_left = 0;
    int state = STRING_START;
    char* input_cp = input;



    if (input[strlen(input) - 1] == '"') { //pokud se jedna o literal uvozeny """string"""
        input[strlen(input) - 1] = '\0';
        input[strlen(input) - 1] = '\0';
        input[strlen(input) - 1] = '\0';
    }
    else //pokud se jedna o literal uvozeny 'string'
        input[strlen(input) - 1] = '\0'; //posledni znak (apostrof) nahradim ukoncujicim znakem

    while (*input_cp) {
        switch (state){
            case STRING_START:
                if (*input_cp == '\\')
                    state = ESCAPE_CHAR;
                else if (*input_cp == '\r') { //pokud se jedna o CR, tak musí nasledovat LF
                    *(input_cp - shift_left) = *input_cp;
                    if (*(input_cp + 1) != '\n') //zkontrolujeme, zda opravdu nasleduje LF
                        printError(ERROR_LEX, NULL);
                }
                else
                    *(input_cp - shift_left) = *input_cp; //kopirovani normalnich znaku na prislusne misto
                break;

            case ESCAPE_CHAR:
                if (*input_cp == 'x') {
                    shift_left += 3; //jelikoz nahradime ctyr-znakovou sekvenci za jeden znak
                    state = HEX_CHAR;
                    break;
                }

                shift_left += 1; //jelikoz nahradime dvou-znakovou sekvenci za jeden znak
                if (*input_cp == '\"')
                    *(input_cp - shift_left) = '\"';
                else if (*input_cp == '\'')
                    *(input_cp - shift_left) = '\'';
                else if (*input_cp == 'n')
                    *(input_cp - shift_left) = '\n';
                else if (*input_cp == 't')
                    *(input_cp - shift_left) = '\t';
                else if (*input_cp == '\\') //jina moznost nenastane, to je osetreno v hlavnim fsm
                    *(input_cp - shift_left) = '\\';
                else {
                    shift_left--;
                    *(input_cp - shift_left -1) = '\\';
                    *(input_cp - shift_left) = *input_cp;
                }
                state = STRING_START;
                break;

            case HEX_CHAR:
                input_cp++; //musime rucne posunout, jelikoz zpracovavame dva znaky najednou
                //vezmeme aktualni a predchozi znak, prevedeme je do desitkove soustavy a ulozime do pole
                *(input_cp - shift_left) = two_digit_hex_to_dec(*(input_cp - 1), *input_cp);
                state = STRING_START;
                break;
        } //switch
        input_cp++;
    } //while

    *(input_cp - shift_left) = '\0'; //posunuti ukoncovaciho znaku

    //realokace podle aktualni delky, delka noveho retezce je <= delka stareho
    return input;
}


char *load_to_str(Stack* stack, int char_counter){

    printf("we are gere");
    char *str = (char *)malloc(char_counter + 1);

    if (str == NULL)
        printErrorIn(ERROR_PROGRAM);
    
    //zkopiruje chars_loaded znaku z souboru do str

    printf("\n%d\n", char_counter);
    for (int i = char_counter; i > 0; i--){
         str[i] = stack->top->data;
         printf("%c", stack->top->data);
         stack->top = stack->top->next;
         

    }
     str[char_counter +1] = '\0';


    printf("\n\n");

    for(int i = 0; i <=char_counter+1 ; i++){
    printf("%c", str[i]);
        
    }
    printf("\n\n");

    return str;
}




int iskeyword(char *s){
    if (!strcmp((const char*)s, "else")){
        return KEY_ELSE;
     } 
    if (!strcmp((const char*)s, "float")){
        return KEY_FLOAT;
    }   
    if (!strcmp((const char*)s, "function")){
        return KEY_FUNCTION;
    }    
    if (!strcmp((const char*)s, "if")){
        return KEY_IF;
    }  
    if (!strcmp((const char*)s, "while")){
        return KEY_WHILE;
    }   
    if (!strcmp((const char*)s, "int")){
        return KEY_INT;
    }   
    if (!strcmp((const char*)s, "null")){
        return KEY_NULL;
    }   
    if (!strcmp((const char*)s, "return")){
        return KEY_RETURN;
    }   
    if (!strcmp((const char*)s, "string")){
        return KEY_STRING;
    }     
    if (!strcmp((const char*)s, "void")){
        return KEY_VOID;
    }
        
    return NOT_KEY; // if not keyword
}






token_t create_token(int token_id, token_value value){
    token_t token;
    token.type = token_id;
    int tmp = 0;




  

    switch (token_id){
        case TK_DOUBLE:
            token.value.double_value = strtod(value.string , NULL);
            if (errno == ERANGE)
                printError(ERROR_SEM_TYPE, &token); //pri preteceni nebo podteceni
            free(value.string);
            break;

        case ID:
            if (tmp == iskeyword(value.string)) { //pokud se jedna o klicove slovo, tak je v tmp jeho id, jinak 0
                token.type = TK_KWRD;
                token.value.keyword_value = tmp;
                free(value.string);
            }
            else     //pokud se jedna o identifikator pouze uloz jeho jmeno
                token.value.string = value.string;
            break;

        case TK_STRING:
            token.value.string = convert_to_str(value.string);
            break;

        case TK_INT:
            token.value.int_value = str_to_int(value.string);
            break;


        default:
            token.value = value;
            break;
    } //switch

    return token;
}



//===========================================================================

// printf("            \n=======================\nhow fat can this shit go ?\n==========================");





token_t get_token(FILE *src_file) 
{
    state = START;
    token_value value;
    static Stack stack;
    char actual_charr;
    int char_counter = 0;\
    static int first_token = 1;


    int i = 1;

    if (first_token == 1) {
            first_token = 0; //dalsi tokeny uz se neprovede
            Stack_Init(&stack);
            Stack_Push(&stack, 0); //vlozeni pomocne 0
            char_counter++;    
    }

    while(1){

            actual_charr = getc(stdin);    //nacteni dalsiho znaku
            printf("actual character: %c \n", actual_charr);
            

        i++;
        

        switch (state)
        {
        case START:

            printf("====== START STATE \n");
            printf("actual charr: %c\n", actual_charr );

            if (actual_charr == '\n') {
                printf("this is new line");
                return create_token(EOLine, value);
            }
            if (actual_charr == '<'){state = LESS;break;}
            if (actual_charr  == '_' || ((actual_charr >= 'a' && actual_charr <= 'z') || (actual_charr >= 'A' && actual_charr <= 'Z') || (actual_charr == '_'))) {Stack_Push(&stack, actual_charr);
                char_counter++;state = ID;break;}
            if (actual_charr  == '?' ) {state = TYPE_ID;break;}
            if (actual_charr == ' ' || actual_charr == '\t') {state = LESS; break;}
            if (actual_charr == '(') {state = PAR_LEFT;ungetc(actual_charr, stdin); break;}
            if (actual_charr == ')') {state = PAR_RIGHT;ungetc(actual_charr, stdin); break;}
            if (actual_charr == '=') {state = EQ; break;}
            if (isdigit(actual_charr)) {state = NUMBER; char_counter++; Stack_Push(&stack, actual_charr); break;}
            if (actual_charr == ';') {return create_token(SEMI_COLON,value);}

        case ID:
            printf("====== ID STATE\n");
            printf("actual charr: %c\n", actual_charr );
            if (actual_charr  == '_' || ((actual_charr >= 'a' && actual_charr <= 'z') || (actual_charr >= 'A' && actual_charr <= 'Z') || (actual_charr == '_')) ){
                printf("pushiiing\n");
                Stack_Push(&stack, actual_charr);
                char_counter++;                
                state = ID;
            }
            else {
                Stack_Print_C(&stack);  
                value.string = load_to_str(&stack, char_counter);{
                    ungetc(actual_charr, stdin);
                    return create_token(ID, value);
                }
                
            }
            break;
            
        case LESS:
                printf("====== LESS STATE\n");
                // if (symbol == '=') {
                //     token->type = STT_LessEqual;
                //     return;
                // }

                if (actual_charr == '?') {
                    state = HEADER;
                    Stack_Push(&stack, actual_charr);
                }
                // else {
                //     lastChar = symbol;
                //     charStreamSwitch = 0;
                //     token->type = STT_Less;
                //     return;
                // }
                break;
        case NUMBER:
                printf("====== NUMBER STATE\n");

                 if (actual_charr >= '0' && actual_charr <= '9') {
                    state = NUMBER;
                    Stack_Push(&stack, actual_charr);
                }
                // else if (actual_charr == '.') {
                //     state = SS_DoubleDecPoint;
                //     stringPush(tokenStr, symbol);
                // }
                // else if ((actual_charr == 'e') || (actual_charr == 'E')) {
                //     state = SS_DoubleExponent;
                //     stringPush(tokenStr, actual_charr);
                // }
                else {
                    ungetc(actual_charr,stdin);
                    create_token(TK_INT,value);
                    
                }
                break;
        
        case HEADER:{
            printf("====== HEADER STATE\n");

            if(actual_charr == 'p'){
                char string_header[] = "hp";
                for(int i = 0; i <= 1; i++){
                    actual_charr = getc(stdin);
                    if(actual_charr != string_header[i]){
                            printf("not the same:\n" );
                        printErrorIn(ERR_LEX);
                    } 
                } 
                 actual_charr = getc(stdin);    
                    if(isspace(actual_charr)){
                    return create_token(TK_PHP,value);
                    }
            }
            else{
                printErrorIn(ERR_LEX);
            }
            break;
        }


        case PAR_LEFT:{
            printf("====== PAR LEFT STATE\n");

            return create_token(TK_PAR_LEFT, value);
        }

        case PAR_RIGHT:{
            printf("====== PAR RIGHT STATE\n");

            return create_token(TK_PAR_RIGHT, value);
        }

        case EQ:{
            printf("====== EQ STATE\n");

            actual_charr = getc(stdin); 
            if(actual_charr == '='){
                state = TK_COMPARSION;
                break;
            }
            ungetc(actual_charr,stdin);
            return create_token(TK_PAR_LEFT, value);
        }
        case TK_COMPARSION:{
            printf("====== TK_COMPARSION STATE\n");

            actual_charr = getc(stdin); 
            if(actual_charr != '='){
                ungetc(actual_charr,stdin);
                printErrorIn(ERR_LEX);
            }
            else{
                return create_token(TK_PAR_LEFT, value);
            }
            
        }


        default:
            printErrorIn(ERROR_PROGRAM);
            break;


        if(i == 20){
            Stack_Print_C(&stack);
            Stack_Destroy(&stack);
            return create_token(TK_EOL, value);  
        }
        
    }

}// while 

    return create_token(TK_EOL, value); 
    printf("%c",actual_charr); 

}



