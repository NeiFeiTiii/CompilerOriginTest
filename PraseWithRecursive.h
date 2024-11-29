//
// Created by ddd on 2024/11/22.
//
#ifndef COMPLIETEST2_PRASEWITHRECURSIVE_H
#define COMPLIETEST2_PRASEWITHRECURSIVE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lexical.h"
#define MAXLENGTH 100
extern int currentToken;

void ErrorPrint(const char *message);   // 报错




typedef struct {
    char op[MAXLENGTH];
    char arg1[MAXLENGTH];
    char arg2[MAXLENGTH];
    char result[MAXLENGTH];
} QUATERNION;   /*存放四元式的数组*/

typedef union {
    char Val1[MAXLENGTH];
    int Val2;
    float Val3;
    char Val4[5];
} WORDCONTENT;

typedef struct {
    TokenType Class;
    WORDCONTENT Value;
} WORD;     //  此处为了和前面实验一二对应，修改了CLASS的变量类型

void scanner(); // 获取下一个单词符号
void GEN(char *Op, char *Arg1, char *Arg2, char *Result);   //生成新的四元式
char *NewTemp(void);    //更新Tn
char *E(void);
char *T(void);
char *F(void);

int PraseWithRecursive();

#endif //COMPLIETEST2_PRASEWITHRECURSIVE_H

