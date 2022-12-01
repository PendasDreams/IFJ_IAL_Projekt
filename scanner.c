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


int get_char_type(char c){
	// 0-9
	if( c >= '0' && c <= '9' ){
		return 1;
	}

	// a-z,A-Z
	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
		return 2;
	}

	return 0;
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


char *load_to_str(Stack* stack, int char_counter, char* str){

    //printf("we are gere");
    //printf("char counter = %d",char_counter);
    

    if (str == NULL)
        printErrorIn(ERROR_PROGRAM);
    
    //zkopiruje chars_loaded znaku z souboru do str

    //printf("\n%d\n", char_counter);
    for (int i = char_counter -1; i >= 0; i--){
         str[i] = stack->top->data;
         //printf("%c", stack->top->data);
         stack->top = stack->top->next;
         

    }


    // //printf("\n\n");

    // for(int i = 0; i <=char_counter  ; i++){
    // //printf("%c) %d\n", str[i], i);
        
    // }
    // //printf("\n\n");

    return str;
}




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
        //printf("\nwe are pusssieees\n");
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
        //printf("\nis keydeclare\n");
        return KEY_DECLARE;
    }
        
    return NOT_KEY; // if not keyword
}






token_t create_token(int token_id, token_value value, Stack* stack){
    token_t token;
    token.type = token_id;
    int tmp = 0;
    char skipper;



  

    switch (token_id){
        case TK_FLOAT:
            token.value.double_value = strtod(value.string , NULL);
            if (errno == ERANGE)
                printError(ERROR_SEM_TYPE, &token); //pri preteceni nebo podteceni
            printf("TOKEN: %s\n", value.string);
            free(value.string);
            break;

        case TK_ID:
            //printf("\n string >>%s\n", value.string);
            printf("TOKEN: %s\n", value.string);

            if(iskeyword(value.string) ==KEY_DECLARE){
                //printf("\n creating ID token\n");
                while(skipper != ';'){
                    skipper =getc(stdin);
                }
                token.type = TK_PHP;
                break;
            }
            if (tmp != iskeyword(value.string)) { //pokud se jedna o klicove slovo, tak je v tmp jeho id, jinak 0
                token.type = TK_KWRD;
                token.value.keyword_value = iskeyword(value.string);
                printf("\nTOKEN KW VALUE: %d\n",token.value.keyword_value);
                free(value.string);
            }
            else     //pokud se jedna o identifikator pouze uloz jeho jmeno
                token.value.string = value.string;
            break;

        case TK_STRING:
            token.value.string = convert_to_str(value.string);
            printf("TOKEN: %s\n", value.string);

            break;

        case TK_INT:
            token.value.int_value = str_to_int(value.string);
            printf("TOKEN: %s\n", value.string);
            break;


        default:
            token.value = value;
            break;
    } //switch


    printf("TYP TOKENU: %d\n", token.type);
    
    Stack_Destroy(stack);
    return token;
}



//===========================================================================

// //printf("            \n=======================\nhow fat can this shit go ?\n==========================");





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
            first_token = 0; //dalsi tokeny uz se neprovede
            Stack_Init(&stack);
            //printf("\n\nstack was initiliyed\n\n");

            char_counter++;    
    }

    while(1){

            actual_charr = getc(stdin);    //nacteni dalsiho znaku
            //printf("\nWHILE LOOP START   actual character: %c ", actual_charr);
            

        i++;
        

        switch (state)
        {
        case START:

            //printf("\n====== START STATE ");
            //printf("actual charracter: %c", actual_charr );

            if (actual_charr == '\n') {
                //printf("\nthis is new line\n");
                state = START;
                break;
            }
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
            if (actual_charr == '/') {state = DIV; break;}
            if (actual_charr == '"') {state = STRING; break;}
            if (actual_charr == '$') {state = DOLLAR; break;}
            if (actual_charr == ':') {state = COLON; break;}
            if (actual_charr == '*') {state = MUL; break;}
            if (actual_charr == ',') {state = COMMA; break;}
            if (actual_charr == '.') {state = CONCAT; break;}
            if (actual_charr == '!') {state = NEG_COMPARSION;ungetc(actual_charr,stdin); break;}


        case TYPE_ID:
            //printf("\n====== TYPE_ID STATE");
            if(actual_charr == '>'){
                return create_token(TK_FOOTER,value,&stack);

            }
            else{
                // tady bude typ of value
            }

        case NEG_COMPARSION:
            //printf("\n====== NEG_COMPARSION STATE");
            for(int i = 0; i <= 1; i++){
                actual_charr = getc(stdin);
                if(actual_charr != '='){
                    printErrorIn(ERROR_LEX);
                }
            }
            return create_token(TK_NEG_COMPARSION,value,&stack);

        case COMMA:
            //printf("\n====== COMMA STATE");
            return create_token(TK_COMMA,value,&stack);
        case CONCAT:
            //printf("\n====== CONCAT STATE");
            return create_token(TK_CONCAT,value,&stack);

        case MUL:
            //printf("\n====== MUL STATE");
            return create_token(TK_MUL,value,&stack);

        case COLON:
            //printf("\n====== COLON STATE");
            return create_token(TK_COLON,value,&stack);


        case DOLLAR:
            //printf("\n====== DOLLAR STATE");
                if (isalpha(actual_charr) || actual_charr == '_') {
                    state = VARIABLE;
                    char_counter++;
                    Stack_Push(&stack,actual_charr);
                    break;
                }
                else {
                printErrorIn(ERROR_LEX);
                }
                break;

         case EOFile: {
                //printf("\n====== EOF STATE");
                
                    return create_token(TK_EOF,value,&stack);
                
            }
            
         case VARIABLE: {
                //printf("\n====== VARIABLE STATE");
                if (isalnum(actual_charr) || (actual_charr == '_')) {
                    char_counter++;
                    Stack_Push(&stack,actual_charr);
                    state = VARIABLE;
                }
                else {
                     //Stack_Print_C(&stack);
                    char *str = (char *)malloc(char_counter + 1);  
                    value.string = load_to_str(&stack, char_counter,str);
                    //printf("\n%s\n",value.string);
                    free(str);
                    ungetc(actual_charr, stdin); 
                    return create_token(TK_VARIABLE,value,&stack);
                }
                break;
            }

        case ID:
            //printf("\n====== ID STATE  ");
            //printf("actual charr: %c\n", actual_charr );
            if (actual_charr  == '_' || ((actual_charr >= 'a' && actual_charr <= 'z') || (actual_charr >= 'A' && actual_charr <= 'Z') || (actual_charr == '_')) ){
                //printf("pushiiing\n");
                Stack_Push(&stack, actual_charr);
                char_counter++;                
                state = ID;
            }
            else {
                 //Stack_Print_C(&stack);  
                char *str = (char *)malloc(char_counter + 1);  
                value.string = load_to_str(&stack, char_counter,str);
                //printf("\n%s\n",value.string);
                //free(str);
                ungetc(actual_charr, stdin);
                return create_token(TK_ID, value,&stack);
            }
            break;

        case NUMBER:


            //printf("\n====== NUMBER STATE");
            //printf("actual charr: %c\n", actual_charr );
            if (actual_charr >= '0' && actual_charr <= '9') {
                //printf("pushiiing\n");
                state = NUMBER;
                Stack_Push(&stack, actual_charr);
                char_counter++; 
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
                 //Stack_Print_C(&stack);  
               char *str = (char *)malloc(char_counter + 1);  
                value.string = load_to_str(&stack, char_counter,str);
                //printf("\n%s\n",value.string);
                free(str);
                ungetc(actual_charr,stdin);
                return create_token(TK_INT,value,&stack);
                
            }
            break;


        case STRING:{
            //printf("\n====== STRING STATE  ");
            if(actual_charr == EOF){
                printErrorIn(ERROR_LEX);
            }
            if(actual_charr == '"'){
                // Vstup "" -> initializuji prazdny string
                 //Stack_Print_C(&stack); 
                char *str = (char *)malloc(char_counter + 1);  
                value.string = load_to_str(&stack, char_counter,str);
                //printf("\n%s\n",value.string);
                free(str);
                ungetc(actual_charr, stdin);
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
            //printf("\n====== STRING CHECK ASCII STATE  ");
            // Neukonceny string = chyba
            if(actual_charr == EOF){
                printErrorIn(ERROR_LEX);
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
            }
            break;
        }
        case STRING_VALID:{
            //printf("\n====== STRING VALID STATE  ");
				if(actual_charr == EOF){
                printErrorIn(ERROR_LEX);
				}
				if(actual_charr == '\\'){
					state = STRING_BACKSLASH;
					break;
				}
				else if(actual_charr == '"'){
                     //Stack_Print_C(&stack); 
                    char *str = (char *)malloc(char_counter + 1);  
                    value.string = load_to_str(&stack, char_counter,str);
                    //printf("\n%s\n",value.string);
                    free(str);
					return create_token(TK_STRING,value,&stack);
				}
				else {
					state = STRING_CHECK_ASCII;
					ungetc(actual_charr, stdin);
					break;
				}
        }
        case STRING_BACKSLASH:
            //printf("\n====== STRING BACKSLASH STATE  ");
				// Neukonceny string == chyba
				if(actual_charr == EOF){
                printErrorIn(ERROR_LEX);
				}
				if(actual_charr == '"' || actual_charr == '\\' || actual_charr == 'n' || actual_charr == 't'|| actual_charr == '$'){
					state = STRING_BACKSLASH_CORRECT;
					ungetc(actual_charr, stdin);
					break;
				}
				// bude to napr. \123
				else if(get_char_type(actual_charr) == 1){
					state = STRING_BACKSLASH_ASCII;
					ungetc(actual_charr, stdin);
					break;
				}
				else {
                printErrorIn(ERROR_LEX);
				}
        // čísla
        case STRING_BACKSLASH_ASCII:{
            //printf("\n====== STRING BACKSLASH ASCII STATE  ");
				// Vlozim 3 znaky
				char *str = malloc(3 + 1);
				for(int i = 0; i < 3; i++){
					if(get_char_type(actual_charr) == 1){
						str[i] = actual_charr;
					}
					else {
						// Chyba
                printErrorIn(ERROR_LEX);
					}
					actual_charr = getc(stdin);
				}
				// Posledni znak chci vratit
				ungetc(actual_charr,stdin);
				// Nastavim ukoncovaci znak
				str[3] = '\0';
				// Prevod na int
				long value = strtol(str, NULL,10);
				int res = (int)value;
				// Ascii hodnota pouze 1-255
				if(res < 1 || res > 255){
                printErrorIn(ERROR_LEX);
				}
				// Prevedu integer hodnotu ascii znaku na ascii znak
				char finalChar = res;
				free(str);
				Stack_Push(&stack,finalChar);
                char_counter++; 
				state = STRING_VALID;
				break;
			}

        case STRING_BACKSLASH_CORRECT:
                //printf("\n====== STRING_BACKSLASH_CORRECT STATE  ");
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
				// \t
				else {
					Stack_Push(&stack, '\t');
				}
				state = STRING_VALID;
				break;
            
        case LESS:
                //printf("\n====== LESS STATE");
                if (actual_charr == '=') {
                    return create_token(TK_EQ_LESS,value,&stack);
                }

                else if (actual_charr == '?') {
                    state = HEADER;
                    Stack_Push(&stack, actual_charr);
                }
                else if (actual_charr != '=') {
                    return create_token(TK_LESS,value,&stack);
                }
                else {
                    printErrorIn(ERR_LEX);
                }
                break;

        case GREATER:
                //printf("\n====== LESS GREATER");
                if (actual_charr == '=') {
                    return create_token(TK_EQ_GREATER,value,&stack);
                }
                else if(actual_charr != '=') {
                    return create_token(TK_GREATER,value,&stack);
                }
                else{
                    printErrorIn(ERR_LEX);
                }
                break;

        
        case HEADER:{
            //printf("\n====== HEADER STATE");

            if(actual_charr == 'p'){
                char string_header[] = "hp";
                for(int i = 0; i <= 1; i++){
                    actual_charr = getc(stdin);
                    if(actual_charr != string_header[i]){
                            //printf("not the same:\n" );
                        printErrorIn(ERR_LEX);
                    } 
                } 
                 actual_charr = getc(stdin);    
                    if(isspace(actual_charr)){
                    return create_token(TK_PHP,value,&stack);
                    }
            }
            else{
                printErrorIn(ERR_LEX);
            }
            break;
        }


        case PAR_LEFT:{
            //printf("\n====== PAR LEFT STATE");

            return create_token(TK_PAR_LEFT, value,&stack);
        }
        case SPACE:{
            //printf("\n====== SPACE STATE");
            while(isspace(actual_charr) || actual_charr == '\t'){
                actual_charr = getc(stdin);
                
            }
            ungetc(actual_charr,stdin);
            state = START;
            break;
            
        }

        case PAR_RIGHT:{
            //printf("\n====== PAR RIGHT STATE");

            return create_token(TK_PAR_RIGHT, value,&stack);
        }
        case COMMENT:{
            //printf("\n====== COMMENT STATE\n");

            while(actual_charr != '\n'){
                //printf("COMM READING > %c\n", actual_charr);      

                actual_charr = getc(stdin);    
            }
            state = START;
            break;
        }
        case BLOCK_COMMENT:{
            //printf("===== BLOCK_COMMENT STATE\n");
            while(actual_charr != '*'){
                actual_charr = getc(stdin);   
                //printf("BLOCK COMM READING > %c\n", actual_charr);      
 
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
            //printf("====== DIV STATE\n");
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
            //printf("====== EQ STATE\n");

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
            //printf("====== TK_COMPARSION STATE\n");

            actual_charr = getc(stdin); 
            if(actual_charr != '='){
                printErrorIn(ERR_LEX);
            }
            else{
                return create_token(TK_COMPARSION, value,&stack);
            }
            
        }


        default:
            printErrorIn(ERROR_PROGRAM);
            break;
        
    }

}// while 

    

    return create_token(TK_EOL, value,&stack); 

}



