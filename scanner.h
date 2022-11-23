/**
 * @file scanner.h
 * @author 
 * @brief Lexikalni analyzator
 */


// keywords

typedef enum TK_KW{
    
    KEY_BOOL = 100,
    
    KEY_ELSE = 101, 
    KEY_FLOAT = 102, 
    KEY_FUNCTION = 103, 
    KEY_IF = 104, 
    KEY_INT = 105, 
    KEY_NULL = 106, 
    KEY_RETURN = 107, 
    KEY_STRING = 108,
    KEY_VOID = 109, 
    KEY_WHILE = 110,
    KEY_DECLARE = 111

} TK_KW_T;


// typ tokenu

typedef enum TK_TYPE{
    TK_ID,
    TK_KW,
    TK_ADD,
    TK_SUB,
    TK_MUL,
    TK_DIV,
    TK_EQ,
    TK_AND,                 
    TK_OR,                  
    TK_NOT,   
    TK_GREATER,
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
    TK_CONCAT,
    TK_EOF,
    TK_EOL,
    TK_PHP

} TK_TYPE;


// stav automatu

typedef enum FSM_STATE {
    START,                  
    DOLLAR,                 // $ variable identificator
    ID,
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
    SEMI_COLON,             // ;
    COLON,                  // :
    LINE,                   // ,
    CONCAT,                 // concatenation '.'
    COMMENT,                // commentary
    BLOCK_COMMENT,          // block commentary
    SPACE,                  // space ' '
    STRING_LITERAL,         // string
    RETURN,
    EOFile,
    EOLine,
    HEADER,
    HEADER1,
    HEADER2,
    HEADER3
} STATE;

typedef struct TOKEN{
    TK_TYPE type;
    TK_KW_T keyword;
    char *data;
    int line;
} TOKEN;

// typedef struct token
// {
//     TK_TYPE TokenType;
//     char Token[20]; // TODO dynamicky alokovany pole
//     int line;
// } token_T;


void  SpracujZnak(TOKEN *token);


