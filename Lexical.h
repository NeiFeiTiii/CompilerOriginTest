#ifndef LEXICAL_H
#define LEXICAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 100
#define TOKEN_NAMES_SIZE 28

void out(int code, char *token);
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

extern const char* token_names[];
extern char *TOKEN;
extern FILE *output_file;

int lookup(char *token);
void scanner_example(FILE *fp);
int Lexical();



#endif // LEXICAL_H