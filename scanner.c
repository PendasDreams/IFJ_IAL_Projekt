/**
 * @file scanner.c
 * @author Denis Novosád xnovos14
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



int state;
int line_count = 1;
int first_token = 1;



int printErrorIn(error_code_t err){
        fprintf(stderr, " %d Inner programm error\n", err);
        return err;
}

// vrátí decimální hodnotu hexadecimální hodnoty 
char hex_to_dec (char first, char second) {

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

// vrátí int ze stringu
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


// nahraje znaky ze zásobníku do stringu 
char *load_to_str(Stack* stack, int char_counter, char* str){

    
    if (str == NULL)
        printErrorIn(ERROR_PROGRAM);

    for (int i = char_counter -1; i >= 0; i--){
         str[i] = stack->top->data;
         stack->top = stack->top->next;
    }
    return str;
}



// funkce na ověření, zda je identifikátor klíčové slovo
int iskeyword(char *s){


    if (strcmp((const char*)s, "else") == 0){
        return KEY_ELSE;
     } 
    if (strcmp((const char*)s, "float")== 0){
        return KEY_FLOAT;
    }   
    if (strcmp((const char*)s, "function")== 0){
        return KEY_FUNCTION;
    }    
    if (strcmp((const char*)s, "if") == 0){
        return KEY_IF;
    }  
    if (strcmp((const char*)s, "while")== 0){
        return KEY_WHILE;
    }   
    if (strcmp((const char*)s, "int")== 0){
        return KEY_INT;
    }   
    if (strcmp((const char*)s, "null")== 0){
        return KEY_NULL;
    }   
    if (strcmp((const char*)s, "return")== 0){
        return KEY_RETURN;
    }   
    if (strcmp((const char*)s, "string")== 0){
        return KEY_STRING;
    }     
    if (strcmp((const char*)s, "void")== 0){
        return KEY_VOID;
    }
    if (strcmp((const char*)s, "declare")== 0){
        return KEY_DECLARE;
    }
        
    return NOT_KEY;
}





// funkce fytvoří token
token_t create_token(int token_id, token_value value, Stack* stack){
    token_t token;
    token.type = token_id;
    int tmp = 0;
    char skipper;

    switch (token_id){

        case TK_FLOAT:
            token.value.double_value = strtod(value.string , NULL);
            if (errno == ERANGE)
                printErrorIn(ERROR_LEX);
            free(value.string);
            break;

        case TK_ID:

            if(iskeyword(value.string) == KEY_DECLARE){
                while(skipper != ';'){
                    skipper =getc(stdin);
                }
                token.type = TK_PHP;
                break;
            }
            if (tmp != iskeyword(value.string)) {
                token.type = TK_KWRD;
                token.value.keyword_value = iskeyword(value.string);
                free(value.string);
            }
            else 
                token.value.string = value.string;
            break;

        case TK_INT:
            token.value.int_value = str_to_int(value.string);
            break;


        default:
            token.value = value;
            break;
    } //switch
    
    Stack_Destroy(stack);
    return token;
}


// funkce načítá znaky z stdin dokud nenačte všechny znaky potřebné pro vytvořneí tokenu
token_t get_token(FILE *src_file) 
{
    state = START;
    token_value value;
    static Stack stack;
    char actual_charr;
    int char_counter = 0;
    static int first_token = 1;


    int i = 1;

    if (first_token == 1) {
            first_token = 0;
            Stack_Init(&stack);
            char_counter++;    
    }

    while(1){

        actual_charr = getc(stdin);   
        
        switch (state)
        {
        case START: // počáteční stav automatu

            if (actual_charr == '\n') {state = START;break;}
            if (actual_charr == EOF) {state = EOFile; break;}
            if (actual_charr == '<'){state = LESS;break;}
            if (actual_charr == '>'){state = GREATER;break;}
            if (actual_charr  == '_' || ((actual_charr >= 'a' && actual_charr <= 'z') || (actual_charr >= 'A' && actual_charr <= 'Z') || (actual_charr == '_'))) {ungetc(actual_charr,stdin);
                state = ID;break;}
            if (actual_charr  == '?' ) {state = TYPE_ID;break;}
            if (actual_charr == ' ' || actual_charr == '\t' || isspace(actual_charr)) {state = SPACE;ungetc(actual_charr,stdin); break;}
            if (actual_charr == '(') {state = PAR_LEFT;ungetc(actual_charr, stdin); break;}
            if (actual_charr == ')') {state = PAR_RIGHT;ungetc(actual_charr, stdin); break;}
            if (actual_charr == '=') {state = EQ;ungetc(actual_charr,stdin); break;}
            if (isdigit(actual_charr)) {state = NUMBER; char_counter++; Stack_Push(&stack, actual_charr); break;}
            if (actual_charr == ';') {return create_token(TK_SEMI_COLON,value,&stack);}\
            if (actual_charr == '{') {return create_token(TK_LEFT_CURLY_BRACKET,value,&stack);}
            if (actual_charr == '}') {return create_token(TK_RIGHT_CURLY_BRACKET,value,&stack);}
            if (actual_charr == '[') {return create_token(TK_LEFT_CURLY_BRACKET,value,&stack);}
            if (actual_charr == ']') {return create_token(TK_LEFT_CURLY_BRACKET,value,&stack);}
            if (actual_charr == '/') {state = DIV; break;}
            if (actual_charr == '"') {state = STRING; break;}
            if (actual_charr == '\'') {state = STRING_CHAR; break;}
            if (actual_charr == '$') {state = DOLLAR; break;}
            if (actual_charr == ':') {state = COLON; break;}
            if (actual_charr == '*') {state = MUL; break;}
            if (actual_charr == '+') {state = ADD; break;}
            if (actual_charr == '-') {state = SUB; break;}
            if (actual_charr == '.') {return create_token(TK_CONCAT,value,&stack);}
            if (actual_charr == ',') {state = COMMA; break;}
            if (actual_charr == '!') {state = EXCLA; break;}
            if (actual_charr == '|') {state = OR; break;}
            if (actual_charr == '&') {state = AND; break;}

        case AND:
            if(actual_charr != '&'){
                printErrorIn(ERROR_LEX);
                return create_token(TK_EOF,value,&stack);
            }
            else{
                return create_token(TK_AND,value,&stack);
            }
        case OR:
            if(actual_charr == '|'){
                return create_token(TK_OR,value,&stack);
            }
            else{
                ungetc(actual_charr,stdin);
                return create_token(TK_BITWISE,value,&stack);
            }
            
        case ADD:
            return create_token(TK_ADD,value,&stack);

        case SUB:
            return create_token(TK_SUB,value,&stack);

        case TYPE_ID:
            if(actual_charr == '>'){
                return create_token(TK_FOOTER,value,&stack);
            }
            else{
                state = ID;
            }

        case EXCLA:
            if(actual_charr == '='){
                state = NEG_COMPARSION;
                break;
            }
            else{
                ungetc(actual_charr,stdin);
                return create_token(TK_NOT,value,&stack);
            }

        case NEG_COMPARSION:
            if (actual_charr == '=') {
                return create_token(TK_NEG_COMPARSION,value,&stack);
            }
            else{
                printErrorIn(ERR_LEX);
                return create_token(TK_EOF,value,&stack);
            }

        case COMMA:
            return create_token(TK_COMMA,value,&stack);
        
        case MUL:
            return create_token(TK_MUL,value,&stack);

        case COLON:
            return create_token(TK_COLON,value,&stack);


        case DOLLAR:
            if (isalpha(actual_charr) || actual_charr == '_') {
                state = VARIABLE;
                char_counter++;
                Stack_Push(&stack,actual_charr);
                break;
            }
            else {
                printErrorIn(ERROR_LEX);
                return create_token(TK_EOF,value,&stack);
            }
            break;

         case EOFile: {                
            return create_token(TK_EOF,value,&stack);
                
            }
            
         case VARIABLE: {
                if (isalnum(actual_charr) || (actual_charr == '_')) {
                    char_counter++;
                    Stack_Push(&stack,actual_charr);
                    state = VARIABLE;
                }
                else {
                    Stack_Print_C(&stack);
                    char *str = (char *)malloc(char_counter + 1);  
                    value.string = load_to_str(&stack, char_counter,str);
                    free(str);
                    ungetc(actual_charr, stdin); 
                    return create_token(TK_VARIABLE,value,&stack);
                }
                break;
            }

        case ID:
            if (actual_charr  == '_' || ((actual_charr >= 'a' && actual_charr <= 'z') || (actual_charr >= 'A' && actual_charr <= 'Z') || (actual_charr == '_')) ){
                Stack_Push(&stack, actual_charr);
                char_counter++;                
                state = ID;
            }
            else {
                Stack_Print_C(&stack);  
                char *str = (char *)malloc(char_counter + 1);  
                value.string = load_to_str(&stack, char_counter,str);
                free(str);
                ungetc(actual_charr, stdin);
                return create_token(TK_ID, value,&stack);
            }
            break;

        case NUMBER:

            if (isdigit(actual_charr)) {
                state = NUMBER;
                Stack_Push(&stack, actual_charr);
                char_counter++; 
            }
            else if (actual_charr == '.') { 
                state = CONCAT;
                char_counter++; 
                Stack_Push(&stack, actual_charr);
            }
            else if ((actual_charr == 'e') || (actual_charr == 'E')) {
                state = FLOAT_EXPONENT;
                char_counter++; 
                Stack_Push(&stack, actual_charr);
            }
            else {
                Stack_Print_C(&stack);  
                ungetc(actual_charr,stdin);
                return create_token(TK_INT,value,&stack);
                
            }
            break;


        case CONCAT:
            if (isalpha(actual_charr)) {
                state = FLOAT_SECOND_PART;
                char_counter++; 
                Stack_Push(&stack, actual_charr);
            }
            else {
                printErrorIn(ERROR_LEX);
                return create_token(TK_EOF,value,&stack);
            }
            break;

        case FLOAT_SECOND_PART: {
            if (isalpha(actual_charr)) {
                state = FLOAT_SECOND_PART;
                char_counter++; 
                Stack_Push(&stack, actual_charr);
            }
            else if (actual_charr == 'e' || actual_charr == 'E') {
                state = FLOAT_EXPONENT;
                char_counter++; 
                Stack_Push(&stack, actual_charr);
            }
            else {

                return create_token(TK_FLOAT,value,&stack);
            }
            break;
            }
        case FLOAT_EXPONENT:
            if (isalpha(actual_charr)) {
                state = FLOAT_EXPONENT_PART;
                char_counter++; 
                Stack_Push(&stack, actual_charr);
            }
            else if ((actual_charr == '+') || (actual_charr == '-')) {
                state = FLOAT_EXPONENT_SIGN;
                char_counter++; 
                Stack_Push(&stack, actual_charr);
            }
            else {
                printErrorIn(ERROR_LEX);
                return create_token(TK_EOF,value,&stack);
            }
            break;

        break;

        case FLOAT_EXPONENT_SIGN:

            if (isalpha(actual_charr)) {
                state = FLOAT_EXPONENT_PART;
                char_counter++; 
                 Stack_Push(&stack, actual_charr);
            }
            else {
                printErrorIn(ERROR_LEX);
                return create_token(TK_EOF,value,&stack);
            }
            break;

        case FLOAT_EXPONENT_PART:

                if (isalpha(actual_charr)) {
                    state = FLOAT_EXPONENT_PART;
                    char_counter++; 
                    Stack_Push(&stack, actual_charr);
                }
                else {
                    return create_token(TK_FLOAT,value,&stack);
                }
                break;

        case STRING:{
            if(actual_charr == EOF){
                printErrorIn(ERROR_LEX);
                return create_token(TK_EOF,value,&stack);
            }
            if(actual_charr == '"'){
                Stack_Print_C(&stack); 
                char *str = (char *)malloc(char_counter + 1);  
                value.string = load_to_str(&stack, char_counter,str);
                free(str);
                return create_token(TK_STRING,value,&stack);
            }
            else {
                state = STRING_CHECK_ASCII;
                char_counter++; 
                Stack_Push(&stack,actual_charr);
                break;
            }
        }
        case STRING_CHECK_ASCII:{
            if(actual_charr == EOF){
                printErrorIn(ERROR_LEX);
                return create_token(TK_EOF,value,&stack);
            }
            if((actual_charr >= ' ') && (actual_charr != '"')){
                if(actual_charr != '\\'){
                    char_counter++; 
                    Stack_Push(&stack,actual_charr);
                } 
                else
                    ungetc(actual_charr, stdin);
                state = STRING_VALID;
                break;
            }
            else {
                printErrorIn(ERROR_LEX);
                return create_token(TK_EOF,value,&stack);
            }
            break;
        }
        case STRING_VALID:{
				if(actual_charr == EOF){
                    printErrorIn(ERROR_LEX);
                    return create_token(TK_EOF,value,&stack);
				}
				if(actual_charr == '\\'){
					state = STRING_BACKSLASH;
					break;
				}
				else if(actual_charr == '"'){
                    Stack_Print_C(&stack); 
                    char *str = (char *)malloc(char_counter + 1);  
                    value.string = load_to_str(&stack, char_counter,str);
                    free(str);
					return create_token(TK_STRING,value,&stack);
				}
				else {
					state = STRING_CHECK_ASCII;
					ungetc(actual_charr, stdin);
					break;
				}
        }
        case STRING_CHAR:
            while(actual_charr != '\''){
                actual_charr = getc(stdin);   
                if(actual_charr == EOF){
                    printErrorIn(ERR_LEX);
                    return create_token(TK_EOF,value,&stack);
            }    
        }
        return create_token(TK_STRING,value,&stack);
        case STRING_BACKSLASH:
				if(actual_charr == EOF){
                    printErrorIn(ERROR_LEX);
                    return create_token(TK_EOF,value,&stack);
				}
				if(actual_charr == '"' || actual_charr == '\\' || actual_charr == 'n' || actual_charr == 't'|| actual_charr == '$' || actual_charr == 'x'){
					state = STRING_BACKSLASH_CORRECT;
					ungetc(actual_charr, stdin);
					break;
				}
				else if(isalpha(actual_charr)){
					state = STRING_BACKSLASH_ASCII;
					ungetc(actual_charr, stdin);
					break;
				}
				else {
                    printErrorIn(ERROR_LEX);
                    return create_token(TK_EOF,value,&stack);
				}
        case STRING_BACKSLASH_ASCII:{
				char *str = malloc(3 + 1);
				for(int i = 0; i < 3; i++){
					if(isalpha(actual_charr)){
						str[i] = actual_charr;
					}
					else {
                printErrorIn(ERROR_LEX);
                return create_token(TK_EOF,value,&stack);
					}
					actual_charr = getc(stdin);
				}
				ungetc(actual_charr,stdin);
				str[3] = '\0';
				long value = strtol(str, NULL,10);
				int res = (int)value;
				if(res < 1 || res > 255){
                printErrorIn(ERROR_LEX);
                token_value values;
                return create_token(TK_EOF,values,&stack);
				}
				free(str);

                char_counter++; 
				state = STRING_VALID;
				break;
			}

        case STRING_BACKSLASH_CORRECT:
                char_counter++; 
				if(actual_charr == '"'){
					Stack_Push(&stack, '\"');
				}
				else if(actual_charr == '\\'){
					Stack_Push(&stack, '\\');
				}
				else if(actual_charr == 'n'){
					Stack_Push(&stack, '\n');
				}
                else if(actual_charr == '$'){
					Stack_Push(&stack, '$');
				}
                else if(actual_charr == 'x'){
                    actual_charr = getc(stdin);
                    for(int i =0; i<=1; i++){

                        if(actual_charr >= 'A' && actual_charr <= 'F'){
                            Stack_Push(&stack, '$');
                            actual_charr = getc(stdin);
                        }
                        else{
                            printErrorIn(ERR_LEX);
                            return create_token(TK_EOF,value,&stack);
                        }
                    } 
                    ungetc(actual_charr,stdin);
				}
				else {
					Stack_Push(&stack, '\t');
				}
				state = STRING_VALID;
				break;
            
        case LESS:
                if (actual_charr == '=') {
                    return create_token(TK_EQ_LESS,value,&stack);
                }

                else if (actual_charr == '?') {
                    state = HEADER;
                    
                }
                else{
                    return create_token(TK_LESS,value,&stack);
                }
                break;

        case GREATER:
                if (actual_charr == '=') {
                    return create_token(TK_EQ_GREATER,value,&stack);
                }
                else{
                    return create_token(TK_GREATER,value,&stack);
                }
                break;

        
        case HEADER:{
            if(actual_charr == 'p'){
                char string_header[] = "hp";
                for(int i = 0; i <= 1; i++){
                    actual_charr = getc(stdin);
                    if(actual_charr != string_header[i]){
                        printErrorIn(ERR_LEX);
                        return create_token(TK_EOF,value,&stack);
                    } 
                } 
                 actual_charr = getc(stdin);    
                    if(isspace(actual_charr)){
                    return create_token(TK_PHP,value,&stack);
                    }
            }
            else{
                printErrorIn(ERR_LEX);
                return create_token(TK_EOF,value,&stack);
            }
            break;
        }

        case PAR_LEFT:{
            return create_token(TK_PAR_LEFT, value,&stack);
        }
        case SPACE:{
            while(isspace(actual_charr) || actual_charr == '\t'){
                actual_charr = getc(stdin);

                if(actual_charr == EOF){
                    printErrorIn(ERR_LEX);
                    return create_token(TK_EOF,value,&stack);
                }                  
            }
            ungetc(actual_charr,stdin);
            state = START;
            break;
            
        }

        case PAR_RIGHT:{
            return create_token(TK_PAR_RIGHT, value,&stack);
        }
        case COMMENT:{

            while(actual_charr != '\n'){

                actual_charr = getc(stdin); 

                if(actual_charr == EOF){
                    printErrorIn(ERR_LEX);
                    return create_token(TK_EOF,value,&stack);
                }            
            }
            state = START;
            break;
        }
        case BLOCK_COMMENT:{
            while(actual_charr != '*'){
                actual_charr = getc(stdin);   
                if(actual_charr == EOF){
                    printErrorIn(ERR_LEX);
                    return create_token(TK_EOF,value,&stack);
                }    
 
            }
            actual_charr = getc(stdin); 
            if(actual_charr != '/'){
                ungetc(actual_charr,stdin);
                state = BLOCK_COMMENT;
                break;
            } 
            else{
                state = START;
            }

        }
        case DIV:{
            if(actual_charr == '/'){
                state = COMMENT;
                ungetc(actual_charr,stdin);
                break;

            }
            else if( actual_charr == '*'){
                state = BLOCK_COMMENT;
                break;
            }
            else{   
                ungetc(actual_charr,stdin);
                return create_token(TK_DIV, value,&stack);
            }
            
        }

        case EQ:{
            actual_charr = getc(stdin); 
            if(actual_charr == '='){
                state = TK_COMPARSION;
                ungetc(actual_charr,stdin);
                break;
            }
            ungetc(actual_charr,stdin);
            return create_token(TK_EQ, value,&stack);
        }
        case TK_COMPARSION:{

            actual_charr = getc(stdin); 
            if(actual_charr != '='){
                printErrorIn(ERR_LEX);
                return create_token(TK_EOF,value,&stack);
            }
            else{
                return create_token(TK_COMPARSION, value,&stack);
            }
            
        }
        default:
            printErrorIn(ERR_LEX);
            return create_token(TK_EOF,value,&stack);
            break;
        
    }

}// while 

}



