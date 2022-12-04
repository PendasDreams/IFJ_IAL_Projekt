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
    KEY_IF,
    KEY_ELSE , 
    KEY_FLOAT, 
    KEY_FUNCTION,
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
    TK_ID,  //0
    TK_KWRD,    //1
    TK_ADD, //2
    TK_SUB, //3
    TK_MUL, //4
    TK_DIV, //5
    TK_EQ,  //6
    TK_AND,     //  7            
    TK_OR,        //     8        
    TK_GREATER,//   9
    TK_LESS,//10
    TK_EQ_GREATER,//  11     
    TK_EQ_LESS,    //   12     
    TK_COMPARSION,   //   13  
    TK_NEG_COMPARSION, //   14      
    TK_PAR_LEFT,  //   15  
    TK_PAR_RIGHT,   //16
    TK_SEMI_COLON,  // 17   
    TK_COLON,       //  18  
    TK_LINE,  //19
    TK_COMMA,   // 20           
    TK_CONCAT,// 21
    TK_EOF,// 22 
    TK_EOL,// 23
    TK_PHP,// 24
    TK_INT, // 25 
    TK_STRING, //  26
    TK_LEFT_CURLY_BRACKET, // 27
    TK_RIGHT_CURLY_BRACKET, //  28
    TK_VARIABLE, // 29
    TK_FOOTER, // 30
    TK_FLOAT, // 31
    TK_ERROR,
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
    EXCLA,
    FLOAT_SECOND_PART,
    FLOAT_DECIMAL_POINT,
    FLOAT_EXPONENT
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





