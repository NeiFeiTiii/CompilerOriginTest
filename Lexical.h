#ifndef LEXICAL_H
#define LEXICAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 100
#define TOKEN_NAMES_SIZE 28

void out(int code, char *token,int line,int column);
void report_error(const char *error_type);

// 定义类别码的助记符
typedef enum {
    ID,  PLUS, MINUS, MUL, DIV, LP, RP,END,
    FOR, IF, ELSE, AND, OR, NOT,
    INT, REAL,STRING,CHAR,OCTAL, HEX,
    EQ, NE, LT, GT, LE, GE,
    IS,BRACKET,DOT,
    UNKNOWN, ENTER
} TokenType;

typedef struct {
    TokenType type;
    char value[100];
    int row;
    int col;
} Token;

extern int line_number;
extern int column_number;

extern char *TOKEN;
extern FILE *output_file;

extern const char* token_names[];


int lookup(char *token);
void scanner_example(FILE *fp);
int Lexical();
int Lexical2();
void scanner_example2(FILE *fp);


#endif // LEXICAL_H