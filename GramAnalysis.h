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

void readTokens(const char *filename);  // 从实验一结果读取Token，存到全局变量tokens数组里面
Token getNextToken();   // 获取下一个单词符号
void retractToken();    // 回退一个单词符号
void parseError(const char *message);   //报错

#endif //COMPLIETEST2_GRAMANALYSIS_H
