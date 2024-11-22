//
// Created by ddd on 2024/11/22.
//
#ifndef COMPLIETEST2_PRASEWITHRECURSIVE_H
#define COMPLIETEST2_PRASEWITHRECURSIVE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GramAnalysis.h"

#define MAXLENGTH 100

typedef struct {
    char op[MAXLENGTH];
    char arg1[MAXLENGTH];
    char arg2[MAXLENGTH];
    char result[MAXLENGTH];
} QUATERNION;

typedef union {
    char Val1[MAXLENGTH];
    int Val2;
    float Val3;
    char Val4[MAXLENGTH];
} WORDCONTENT;

typedef struct {
    TokenType Class;
    WORDCONTENT Value;
} WORD;

void scanner(); // 获取下一个单词符号
void GEN(char *Op, char *Arg1, char *Arg2, char *Result);
char *NewTemp(void);
char *E(void);
char *T(void);
char *F(void);

int PraseWithRecursive();

#endif //COMPLIETEST2_PRASEWITHRECURSIVE_H

