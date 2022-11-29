/**
 * @file scanner.h
 * @author 
 * @brief Lexikalni analyzator
 */



#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>




// keywords

typedef enum{
    NOT_KEY,
    KEY_ELSE , 
    KEY_FLOAT, 
    KEY_FUNCTION, 
    KEY_IF, 
    KEY_INT , 
    KEY_NULL , 
    KEY_RETURN , 
    KEY_STRING ,
    KEY_VOID , 
    KEY_WHILE,
    KEY_DECLARE
    

} TK_KW_T;


// typ tokenu

typedef enum{
    TK_ID,
    TK_KWRD,
    TK_ADD,
    TK_SUB,
    TK_MUL,
    TK_DIV,
    TK_EQ,
    TK_AND,                 
    TK_OR,                  
    TK_NOT,   
    TK_GREATER,
    TK_LESS,
    TK_EQ_GREATER,       
    TK_EQ_LESS,            
    TK_COMPARSION,        
    TK_NEG_COMPARSION,     
    TK_TYPECOMPARSION,      
    TK_NEG_TYPECOMPARSION,      
    TK_PAR_LEFT,              
    TK_PAR_RIGHT,      
    TK_SEMI_COLON,          
    TK_COLON,                 
    TK_LINE,  
    TK_COMMA,               
    TK_CONCAT,
    TK_EOF,
    TK_EOL,
    TK_PHP,
    TK_DOUBLE,
    TK_INT,
    TK_STRING,
    TK_LEFT_CURLY_BRACKET,
    TK_RIGHT_CURLY_BRACKET,
    TK_VARIABLE,
    TK_FOOTER,


} TK_TYPE;


// stav automatu

typedef enum {
    START,                  // START
    DOLLAR,                 // $ variable identificator
    ID,                     // ID
    TYPE_ID,                // TYPE ID
    VARIABLE,               // variable
    NUMBER,                 // number
    DIV,                    // /
    MUL,                    // *
    ADD,                    // +
    SUB,                    // -
    EQ,                     // =
    GREATER,                // >
    LESS,                   // <
    ASSIGN,                 // :=
    EQ_GREATER,             // >=
    EQ_LESS,                // <=
    COMPARSION,             // ==
    NEG_COMPARSION,         // !=
    TYPECOMPARSION,         // ===
    NEG_TYPECOMPARSION,     // !==
    AND,                    // &&
    OR,                     // or
    NOT,                    // not
    PAR_LEFT,               // (
    PAR_RIGHT,              // )
    LEFT_CURLY_BRACKET,     // {
    RIGHT_CURLY_BRACKET,    // }
    SEMI_COLON,             // ;
    COLON,                  // :
    COMMA,                   // ,
    CONCAT,                 // concatenation '.'
    COMMENT,                // commentary
    BLOCK_COMMENT,          // block commentary
    SPACE,                  // space ' '
    STRING,                 // string
    EOFile,                 // EOF
    EOLine,                 // EOL
    HEADER,                 // <?php + declare
    STRING_CHECK_ASCII,     // string
    STRING_VALID,           // string
    STRING_BACKSLASH,       // string
    STRING_BACKSLASH_ASCII, // stirng
    STRING_BACKSLASH_CORRECT,// string
} STATE;



typedef union {
    char* string;
    double double_value;
    int int_value;
    TK_KW_T keyword_value;
    int noparam;
} token_value;


typedef struct token_t {
    TK_TYPE type;
    token_value value;
    int line;
} token_t;

typedef enum {
    STRING_START,
    ESCAPE_CHAR,
    HEX_CHAR
}str_conv_fsm_states;



typedef enum error_code {
    ERROR_OK , // není chyba
    ERROR_LEX,
    ERROR_SYNTAX ,
    ERROR_SEM_DEF,
    ERROR_SEM_PARAM ,
    ERROR_SEM_UNDEF_VAR ,
    ERROR_SEM_FUNC_RET,
    ERROR_SEM_TYPE ,
    ERROR_SEM_OTHER ,
    ERROR_PROGRAM , 
} error_code_t;



typedef enum errCode {
    ERR_OK,
    ERR_LEX,
    ERR_PARSE,
    ERR_SEM_DEF,
    ERR_SEM_ASSIGN,
    ERR_SEM_PARAM,
    ERR_SEM_TYPE,
    ERR_SEM_OTHER,
    ERR_NIL,
    ERR_ZERO
} ErrCode;




int printError(error_code_t err, token_t *token);
int printErrorIn(error_code_t err);






token_t get_token(FILE* src_file);





