//
// Created by ddd on 2024/11/8.
//

#ifndef COMPLIETEST2_GRAMANALYSIS_H
#define COMPLIETEST2_GRAMANALYSIS_H
#include "Lexical.h"

typedef struct {
    TokenType type;
    char value[100];
    int row;
    int col;
} Token;

extern Token tokens[100];
extern int tokenIndex;
extern int currentToken;

void readTokens(const char *filename);
Token getNextToken();
void retractToken();
void parseError(const char *message);

#endif //COMPLIETEST2_GRAMANALYSIS_H
