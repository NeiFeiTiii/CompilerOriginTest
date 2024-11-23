//
// Created by ddd on 2024/11/8.
//

#ifndef COMPLIETEST2_GRAMANALYSIS_H
#define COMPLIETEST2_GRAMANALYSIS_H
#include "Lexical.h"

extern Token tokens[100];
extern int currentToken;

Token getNextToken();   // 获取下一个单词符号
void ErrorPrint(const char *message);   // 报错


#endif //COMPLIETEST2_GRAMANALYSIS_H
